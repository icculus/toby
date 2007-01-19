/*
** $Id: lparser.c,v 2.42 2006/06/05 15:57:59 roberto Exp $
** Lua Parser
** See Copyright Notice in lua.h
*/


#include <string.h>

#define lparser_c
#define LUA_CORE

#include "lua.h"

#include "lcode.h"
#include "ldebug.h"
#include "ldo.h"
#include "lfunc.h"
#include "llex.h"
#include "lmem.h"
#include "lobject.h"
#include "lopcodes.h"
#include "lparser.h"
#include "lstate.h"
#include "lstring.h"
#include "ltable.h"



#define hasmultret(k)		((k) == VCALL || (k) == VVARARG)

#define getlocvar(fs, i)	((fs)->f->locvars[(fs)->actvar[i]])

#define luaY_checklimit(fs,v,l,m)	if ((v)>(l)) errorlimit(fs,l,m)


/*
** nodes for block list (list of active blocks)
*/
typedef struct BlockCnt {
  struct BlockCnt *previous;  /* chain */
  int breaklist;  /* list of jumps out of this loop */
  lu_byte nactvar;  /* # active locals outside the breakable structure */
  lu_byte upval;  /* true if some variable in the block is an upvalue */
  lu_byte isbreakable;  /* true if `block' is a loop */
} BlockCnt;



/*
** prototypes for recursive non-terminal functions
*/
static void chunk (LexState *ls, int mainline);
static void expr (LexState *ls, expdesc *v);


static void anchor_token (LexState *ls) {
  if (ls->t.token == TK_NAME || ls->t.token == TK_STRINGLIT) {
    TString *ts = ls->t.seminfo.ts;
    luaX_newstring(ls, getstr(ts), ts->tsv.len);
  }
}


static void error_expected (LexState *ls, int token) {
  luaX_syntaxerror(ls,
      luaO_pushfstring(ls->L, LUA_QS " expected", luaX_token2str(ls, token)));
}


static void errorlimit (FuncState *fs, int limit, const char *what) {
  const char *msg = (fs->f->linedefined == 0) ?
    luaO_pushfstring(fs->L, "main function has more than %d %s", limit, what) :
    luaO_pushfstring(fs->L, "function at line %d has more than %d %s",
                            fs->f->linedefined, limit, what);
  luaX_lexerror(fs->ls, msg, 0);
}


static int testnext (LexState *ls, int c) {
  if (ls->t.token == c) {
    luaX_next(ls);
    return 1;
  }
  else return 0;
}


static void check (LexState *ls, int c) {
  if (ls->t.token != c)
    error_expected(ls, c);
}

static void checknext (LexState *ls, int c) {
  check(ls, c);
  luaX_next(ls);
}


#define check_condition(ls,c,msg)	{ if (!(c)) luaX_syntaxerror(ls, msg); }



static void check_match (LexState *ls, int what, int who, int where) {
  if (!testnext(ls, what)) {
    if (where == ls->linenumber)
      error_expected(ls, what);
    else {
      luaX_syntaxerror(ls, luaO_pushfstring(ls->L,
             LUA_QS " expected (to close " LUA_QS " at line %d)",
              luaX_token2str(ls, what), luaX_token2str(ls, who), where));
    }
  }
}


static TString *str_checkname (LexState *ls) {
  TString *ts;
  check(ls, TK_NAME);
  ts = ls->t.seminfo.ts;
  luaX_next(ls);
  return ts;
}


static void init_exp (expdesc *e, expkind k, int i) {
  e->f = e->t = NO_JUMP;
  e->k = k;
  e->u.s.info = i;
}


static void codestring (LexState *ls, expdesc *e, TString *s) {
  init_exp(e, VK, luaK_stringK(ls->fs, s));
}


static void checkname(LexState *ls, expdesc *e) {
  codestring(ls, e, str_checkname(ls));
}


static int registerlocalvar (LexState *ls, TString *varname) {
  FuncState *fs = ls->fs;
  Proto *f = fs->f;
  int oldsize = f->sizelocvars;
  luaM_growvector(ls->L, f->locvars, fs->nlocvars, f->sizelocvars,
                  LocVar, SHRT_MAX, "too many local variables");
  while (oldsize < f->sizelocvars) f->locvars[oldsize++].varname = NULL;
  f->locvars[fs->nlocvars].varname = varname;
  luaC_objbarrier(ls->L, f, varname);
  return fs->nlocvars++;
}


#define new_localvarliteral(ls,v,n) \
  new_localvar(ls, luaX_newstring(ls, "" v, (sizeof(v)/sizeof(char))-1), n)


static void new_localvar (LexState *ls, TString *name, int n) {
  FuncState *fs = ls->fs;
  luaY_checklimit(fs, fs->nactvar+n+1, LUAI_MAXVARS, "local variables");
  fs->actvar[fs->nactvar+n] = cast(unsigned short, registerlocalvar(ls, name));
}


static void adjustlocalvars (LexState *ls, int nvars) {
  FuncState *fs = ls->fs;
  fs->nactvar = cast_byte(fs->nactvar + nvars);
  for (; nvars; nvars--) {
    getlocvar(fs, fs->nactvar - nvars).startpc = fs->pc;
  }
}


static void removevars (LexState *ls, int tolevel) {
  FuncState *fs = ls->fs;
  while (fs->nactvar > tolevel)
    getlocvar(fs, --fs->nactvar).endpc = fs->pc;
}


static int indexupvalue (FuncState *fs, TString *name, expdesc *v) {
  int i;
  Proto *f = fs->f;
  int oldsize = f->sizeupvalues;
  for (i=0; i<f->nups; i++) {
    if (fs->upvalues[i].k == v->k && fs->upvalues[i].info == v->u.s.info) {
      lua_assert(f->upvalues[i] == name);
      return i;
    }
  }
  /* new one */
  luaY_checklimit(fs, f->nups + 1, LUAI_MAXUPVALUES, "upvalues");
  luaM_growvector(fs->L, f->upvalues, f->nups, f->sizeupvalues,
                  TString *, MAX_INT, "");
  while (oldsize < f->sizeupvalues) f->upvalues[oldsize++] = NULL;
  f->upvalues[f->nups] = name;
  luaC_objbarrier(fs->L, f, name);
  lua_assert(v->k == VLOCAL || v->k == VUPVAL);
  fs->upvalues[f->nups].k = cast_byte(v->k);
  fs->upvalues[f->nups].info = cast_byte(v->u.s.info);
  return f->nups++;
}


static int searchvar (FuncState *fs, TString *n) {
  int i;
  for (i=fs->nactvar-1; i >= 0; i--) {
    if (n == getlocvar(fs, i).varname)
      return i;
  }
  return -1;  /* not found */
}


static void markupval (FuncState *fs, int level) {
  BlockCnt *bl = fs->bl;
  while (bl && bl->nactvar > level) bl = bl->previous;
  if (bl) bl->upval = 1;
}


static int singlevaraux (FuncState *fs, TString *n, expdesc *var, int base) {
  if (fs == NULL) {  /* no more levels? */
    init_exp(var, VGLOBAL, NO_REG);  /* default is global variable */
    return VGLOBAL;
  }
  else {
    int v = searchvar(fs, n);  /* look up at current level */
    if (v >= 0) {
      init_exp(var, VLOCAL, v);
      if (!base)
        markupval(fs, v);  /* local will be used as an upval */
      return VLOCAL;
    }
    else {  /* not found at current level; try upper one */
      if (singlevaraux(fs->prev, n, var, 0) == VGLOBAL)
        return VGLOBAL;
      var->u.s.info = indexupvalue(fs, n, var);  /* else was LOCAL or UPVAL */
      var->k = VUPVAL;  /* upvalue in this level */
      return VUPVAL;
    }
  }
}


static void lookupvar(LexState *ls, TString *varname, expdesc *var,
                      int create)
{
  FuncState *fs = ls->fs;
  int add_to_table = ls->resolved_table;
  int already_added = 0;
  lua_State *L = ls->L;

  if (singlevaraux(fs, varname, var, 1) == VGLOBAL) {
    /* see if it's an existing global provided in native code... */
    lua_getglobal(ls->L, getstr(varname));
    int missing = lua_isnil(ls->L, -1);
    lua_pop(ls->L, 1);

    if (create) {
      int on_line = 0;
      if (missing) {
        /* see if it's an existing symbol in the user's program. */
        lua_getfield(L, ls->resolved_table, getstr(varname));
        missing = lua_isnil(L, -1);
        if (!missing)
            on_line = lua_tointeger(L, -1);
        lua_pop(L, 1);
      }
      if (!missing) {
        luaX_syntaxerror(ls,
          luaO_pushfstring(ls->L, LUA_QS "already defined on line %d", getstr(varname), on_line));
      }
    } else {   /* add it to a table to resolve after parsing... */
      if (missing)
        add_to_table = ls->to_resolve_table;
      else
        already_added = 1;
    }

    if (!already_added) {
      lua_getfield(L, add_to_table, getstr(varname));
      already_added = !lua_isnil(L, -1);
      lua_pop(L, 1);
      if (!already_added) {
        lua_pushinteger(L, ls->linenumber);
        lua_setfield(L, add_to_table, getstr(varname));
      }
    }
    var->u.s.info = luaK_stringK(fs, varname); /* info points to global name */
  }
}

static void singlevar (LexState *ls, expdesc *var) {
  lookupvar(ls, str_checkname(ls), var, 0);
}


static void adjust_assign (LexState *ls, int nvars, int nexps, expdesc *e) {
  FuncState *fs = ls->fs;
  int extra = nvars - nexps;
  if (hasmultret(e->k)) {
    extra++;  /* includes call itself */
    if (extra < 0) extra = 0;
    luaK_setreturns(fs, e, extra);  /* last exp. provides the difference */
    if (extra > 1) luaK_reserveregs(fs, extra-1);
  }
  else {
    if (e->k != VVOID) luaK_exp2nextreg(fs, e);  /* close last expression */
    if (extra > 0) {
      int reg = fs->freereg;
      luaK_reserveregs(fs, extra);
      luaK_nil(fs, reg, extra);
    }
  }
}


static void enterlevel (LexState *ls) {
  if (++ls->L->nCcalls > LUAI_MAXCCALLS)
	luaX_lexerror(ls, "chunk has too many syntax levels", 0);
}


#define leavelevel(ls)	((ls)->L->nCcalls--)


static void enterblock (FuncState *fs, BlockCnt *bl, lu_byte isbreakable) {
  bl->breaklist = NO_JUMP;
  bl->isbreakable = isbreakable;
  bl->nactvar = fs->nactvar;
  bl->upval = 0;
  bl->previous = fs->bl;
  fs->bl = bl;
  lua_assert(fs->freereg == fs->nactvar);
}


static void leaveblock (FuncState *fs) {
  BlockCnt *bl = fs->bl;
  fs->bl = bl->previous;
  removevars(fs->ls, bl->nactvar);
  if (bl->upval)
    luaK_codeABC(fs, OP_CLOSE, bl->nactvar, 0, 0);
  /* a block either controls scope or breaks (never both) */
  lua_assert(!bl->isbreakable || !bl->upval);
  lua_assert(bl->nactvar == fs->nactvar);
  fs->freereg = fs->nactvar;  /* free registers */
  luaK_patchtohere(fs, bl->breaklist);
}


static void pushclosure (LexState *ls, FuncState *func, expdesc *v) {
  FuncState *fs = ls->fs;
  Proto *f = fs->f;
  int oldsize = f->sizep;
  int i;
  luaM_growvector(ls->L, f->p, fs->np, f->sizep, Proto *,
                  MAXARG_Bx, "constant table overflow");
  while (oldsize < f->sizep) f->p[oldsize++] = NULL;
  f->p[fs->np++] = func->f;
  luaC_objbarrier(ls->L, f, func->f);
  init_exp(v, VRELOCABLE, luaK_codeABx(fs, OP_CLOSURE, 0, fs->np-1));
  for (i=0; i<func->f->nups; i++) {
    OpCode o = (func->upvalues[i].k == VLOCAL) ? OP_MOVE : OP_GETUPVAL;
    luaK_codeABC(fs, o, 0, func->upvalues[i].info, 0);
  }
}


static void open_func (LexState *ls, FuncState *fs) {
  lua_State *L = ls->L;
  Proto *f = luaF_newproto(L);
  fs->f = f;
  fs->prev = ls->fs;  /* linked list of funcstates */
  fs->ls = ls;
  fs->L = L;
  ls->fs = fs;
  fs->pc = 0;
  fs->lasttarget = -1;
  fs->jpc = NO_JUMP;
  fs->freereg = 0;
  fs->nk = 0;
  fs->np = 0;
  fs->nlocvars = 0;
  fs->nactvar = 0;
  fs->bl = NULL;
  f->source = ls->source;
  f->maxstacksize = 2;  /* registers 0/1 are always valid */
  fs->h = luaH_new(L, 0, 0);
  /* anchor table of constants and prototype (to avoid being collected) */
  sethvalue2s(L, L->top, fs->h);
  incr_top(L);
  setptvalue2s(L, L->top, f);
  incr_top(L);
}


static void close_func (LexState *ls) {
  lua_State *L = ls->L;
  FuncState *fs = ls->fs;
  Proto *f = fs->f;
  removevars(ls, 0);
  luaK_ret(fs, 0, 0);  /* final return */
  luaM_reallocvector(L, f->code, f->sizecode, fs->pc, Instruction);
  f->sizecode = fs->pc;
  luaM_reallocvector(L, f->lineinfo, f->sizelineinfo, fs->pc, int);
  f->sizelineinfo = fs->pc;
  luaM_reallocvector(L, f->k, f->sizek, fs->nk, TValue);
  f->sizek = fs->nk;
  luaM_reallocvector(L, f->p, f->sizep, fs->np, Proto *);
  f->sizep = fs->np;
  luaM_reallocvector(L, f->locvars, f->sizelocvars, fs->nlocvars, LocVar);
  f->sizelocvars = fs->nlocvars;
  luaM_reallocvector(L, f->upvalues, f->sizeupvalues, f->nups, TString *);
  f->sizeupvalues = f->nups;
  lua_assert(luaG_checkcode(f));
  lua_assert(fs->bl == NULL);
  ls->fs = fs->prev;
  L->top -= 2;  /* remove table and prototype from the stack */
  /* last token read was anchored in defunct function; must reanchor it */
  if (fs) anchor_token(ls);
}

static void resolve_toby_functions(LexState *ls) {
  lua_State *L = ls->L;
  const int to_resolve_table = ls->to_resolve_table;
  const int resolved_table = ls->resolved_table;
  int missing = 0;

  lua_pushnil(L);  /* first key for iteration... */
  while (lua_next(L, to_resolve_table)) {  /* replaces key, pushes value. */
    const char *sym = lua_tostring(L, -2);
    const int on_line = lua_tointeger(L, -1);
    lua_getfield(L, resolved_table, sym);
    missing = lua_isnil(L, -1);
    lua_pop(L, 1);

    if (missing) {
      lua_pop(L, 2);  /* remove iterator value and key. */
      luaX_syntaxerror(ls,
        luaO_pushfstring(ls->L, "undefined symbol " LUA_QS " on line %d", sym, on_line));
    }

    lua_pop(L, 1);  /* remove value, keep key for next iteration. */
  }
}


Proto *luaY_parser (lua_State *L, ZIO *z, Mbuffer *buff, const char *name) {
  struct LexState lexstate;
  struct FuncState funcstate;
  lexstate.buff = buff;
  luaX_setinput(L, &lexstate, z, luaS_new(L, name));
  open_func(&lexstate, &funcstate);
  funcstate.f->is_vararg = VARARG_ISVARARG;  /* main func. is always vararg */
  luaX_next(&lexstate);  /* read first token */
  chunk(&lexstate, 1);
  check(&lexstate, TK_EOS);
  close_func(&lexstate);
  resolve_toby_functions(&lexstate);  /* we're less dynamic than Lua.  :) */
  lua_assert(lexstate.to_resolve_table == lua_gettop(L)-1);
  lua_assert(lexstate.resolved_table == lua_gettop(L));
  lua_pop(L, 2);
  lua_assert(funcstate.prev == NULL);
  lua_assert(funcstate.f->nups == 0);
  lua_assert(lexstate.fs == NULL);
  return funcstate.f;
}



/*============================================================*/
/* GRAMMAR RULES */
/*============================================================*/


static void field (LexState *ls, expdesc *v) {
  /* field -> ['.' | ':'] NAME */
  FuncState *fs = ls->fs;
  expdesc key;
  luaK_exp2anyreg(fs, v);
  luaX_next(ls);  /* skip the dot or colon */
  checkname(ls, &key);
  luaK_indexed(fs, v, &key);
}


static void yindex (LexState *ls, expdesc *v) {
  /* index -> '[' expr ']' */
  luaX_next(ls);  /* skip the '[' */
  expr(ls, v);
  luaK_exp2val(ls->fs, v);
  checknext(ls, ']');
}


static void vardeclstat (LexState *ls, int global, int initialize);
static int toby_intrinsic_type(int token, int allownothing);

static void parlist (LexState *ls) {
  /* parlist -> [ param { `,' param } ] */
  FuncState *fs = ls->fs;
  Proto *f = fs->f;
  int nparams = 0;
  f->is_vararg = 0;
  if (ls->t.token != ')') {  /* is `parlist' not empty? */
    do {
      vardeclstat(ls, 0, 0);
      nparams++;
    } while (testnext(ls, ','));
  }
  f->numparams = cast_byte(fs->nactvar - (f->is_vararg & VARARG_HASARG));
  luaK_reserveregs(fs, fs->nactvar);  /* reserve register for parameters */
}

static void check_intrinsic_type(LexState *ls, int allownothing) {
  const int c = toby_intrinsic_type(ls->t.token, allownothing);
  check_condition(ls, c, "data type expected");
}

static void body (LexState *ls, expdesc *e, int line) {
  /* body ->  `(' parlist `)' chunk END */
  FuncState new_fs;
  open_func(ls, &new_fs);
  new_fs.f->linedefined = line;
  checknext(ls, '(');
  parlist(ls);
  checknext(ls, ')');

  checknext(ls, TK_RETURNS);
  check_intrinsic_type(ls, 1);
  luaX_next(ls); /* !!! FIXME: store return type info somewhere... */

  /* Toby requires variable predeclaration at the start of a function. */
  /*  ...maybe we should change that... */
  while (toby_intrinsic_type(ls->t.token, 0))
    vardeclstat(ls, 0, 1);

  chunk(ls, 0);
  new_fs.f->lastlinedefined = ls->linenumber;
  check_match(ls, TK_ENDFUNCTION, TK_FUNCTION, line);
  close_func(ls);
  pushclosure(ls, &new_fs, e);
}


static int explist1 (LexState *ls, expdesc *v) {
  /* explist1 -> expr { `,' expr } */
  int n = 1;  /* at least one expression */
  expr(ls, v);
  while (testnext(ls, ',')) {
    luaK_exp2nextreg(ls->fs, v);
    expr(ls, v);
    n++;
  }
  return n;
}


static void funcargs (LexState *ls, expdesc *f) {
  FuncState *fs = ls->fs;
  expdesc args;
  int base, nparams;
  int line = ls->linenumber;
  switch (ls->t.token) {
    case '(': {  /* funcargs -> `(' [ explist1 ] `)' */
      if (line != ls->lastline)
        luaX_syntaxerror(ls,"ambiguous syntax (function call x new statement)");
      luaX_next(ls);
      if (ls->t.token == ')')  /* arg list is empty? */
        args.k = VVOID;
      else {
        explist1(ls, &args);
        luaK_setmultret(fs, &args);
      }
      check_match(ls, ')', '(', line);
      break;
    }
    default: {
      luaX_syntaxerror(ls, "function arguments expected");
      return;
    }
  }
  lua_assert(f->k == VNONRELOC);
  base = f->u.s.info;  /* base register for call */
  if (hasmultret(args.k))
    nparams = LUA_MULTRET;  /* open call */
  else {
    if (args.k != VVOID)
      luaK_exp2nextreg(fs, &args);  /* close last argument */
    nparams = fs->freereg - (base+1);
  }
  init_exp(f, VCALL, luaK_codeABC(fs, OP_CALL, base, nparams+1, 2));
  luaK_fixline(fs, line);
  fs->freereg = base+1;  /* call remove function and arguments and leaves
                            (unless changed) one result */
}




/*
** {======================================================================
** Expression parsing
** =======================================================================
*/


static void prefixexp (LexState *ls, expdesc *v) {
  /* prefixexp -> NAME | '(' expr ')' */
  switch (ls->t.token) {
    case '(': {
      int line = ls->linenumber;
      luaX_next(ls);
      expr(ls, v);
      check_match(ls, ')', '(', line);
      luaK_dischargevars(ls->fs, v);
      return;
    }
    case TK_NAME: {
      singlevar(ls, v);
      return;
    }
    default: {
      luaX_syntaxerror(ls, "unexpected symbol");
      return;
    }
  }
}


static void primaryexp (LexState *ls, expdesc *v) {
  /* primaryexp ->
        prefixexp { `.' NAME | `[' exp `]' | `:' NAME funcargs | funcargs } */
  FuncState *fs = ls->fs;
  prefixexp(ls, v);
  for (;;) {
    switch (ls->t.token) {
      case '.': {  /* field */
        field(ls, v);
        break;
      }
      case '[': {  /* `[' exp1 `]' */
        expdesc key;
        luaK_exp2anyreg(fs, v);
        yindex(ls, &key);
        luaK_indexed(fs, v, &key);
        break;
      }
      case '(': {  /* funcargs */
        luaK_exp2nextreg(fs, v);
        funcargs(ls, v);
        break;
      }
      default: return;
    }
  }
}


static void simpleexp (LexState *ls, expdesc *v) {
  /* simpleexp -> NUMBER | STRING | NIL | true | false | ... |
                  constructor | FUNCTION body | primaryexp */
  switch (ls->t.token) {
    case TK_NUMBERLIT: {
      init_exp(v, VKNUM, 0);
      v->u.nval = ls->t.seminfo.r;
      break;
    }
    case TK_STRINGLIT: {
      codestring(ls, v, ls->t.seminfo.ts);
      break;
    }
    case TK_TRUE: {
      init_exp(v, VTRUE, 0);
      break;
    }
    case TK_FALSE: {
      init_exp(v, VFALSE, 0);
      break;
    }
    default: {
      primaryexp(ls, v);
      return;
    }
  }
  luaX_next(ls);
}


static UnOpr getunopr (int op) {
  switch (op) {
    case TK_NOT: return OPR_NOT;
    case '-': return OPR_MINUS;
    case '#': return OPR_LEN;
    default: return OPR_NOUNOPR;
  }
}


static BinOpr getbinopr (int op) {
  switch (op) {
    case '+': return OPR_ADD;
    case '-': return OPR_SUB;
    case '*': return OPR_MUL;
    case '/': return OPR_DIV;
    case '%': return OPR_MOD;
    case '^': return OPR_POW;
    case TK_CONCAT: return OPR_CONCAT;
    case TK_NE: return OPR_NE;
    case TK_EQ: return OPR_EQ;
    case '<': return OPR_LT;
    case TK_LE: return OPR_LE;
    case '>': return OPR_GT;
    case TK_GE: return OPR_GE;
    case TK_AND: return OPR_AND;
    case TK_OR: return OPR_OR;
    default: return OPR_NOBINOPR;
  }
}


static const struct {
  lu_byte left;  /* left priority for each binary operator */
  lu_byte right; /* right priority */
} priority[] = {  /* ORDER OPR */
   {6, 6}, {6, 6}, {7, 7}, {7, 7}, {7, 7},  /* `+' `-' `/' `%' */
   {10, 9}, {5, 4},                 /* power and concat (right associative) */
   {3, 3}, {3, 3},                  /* equality and inequality */
   {3, 3}, {3, 3}, {3, 3}, {3, 3},  /* order */
   {2, 2}, {1, 1}                   /* logical (and/or) */
};

#define UNARY_PRIORITY	8  /* priority for unary operators */


/*
** subexpr -> (simpleexp | unop subexpr) { binop subexpr }
** where `binop' is any binary operator with a priority higher than `limit'
*/
static BinOpr subexpr (LexState *ls, expdesc *v, unsigned int limit) {
  BinOpr op;
  UnOpr uop;
  enterlevel(ls);
  uop = getunopr(ls->t.token);
  if (uop != OPR_NOUNOPR) {
    luaX_next(ls);
    subexpr(ls, v, UNARY_PRIORITY);
    luaK_prefix(ls->fs, uop, v);
  }
  else simpleexp(ls, v);
  /* expand while operators have priorities higher than `limit' */
  op = getbinopr(ls->t.token);
  while (op != OPR_NOBINOPR && priority[op].left > limit) {
    expdesc v2;
    BinOpr nextop;
    luaX_next(ls);
    luaK_infix(ls->fs, op, v);
    /* read sub-expression with higher priority */
    nextop = subexpr(ls, &v2, priority[op].right);
    luaK_posfix(ls->fs, op, v, &v2);
    op = nextop;
  }
  leavelevel(ls);
  return op;  /* return first untreated operator */
}


static void expr (LexState *ls, expdesc *v) {
  subexpr(ls, v, 0);
}

/* }==================================================================== */



/*
** {======================================================================
** Rules for Statements
** =======================================================================
*/


static int block_follow (int token) {
  switch (token) {
    case TK_ELSE:
    case TK_ELSEIF:
    case TK_ENDFUNCTION:
    case TK_ENDIF:
    case TK_ENDFOR:
    case TK_ENDWHILE:
    case TK_EOS:
      return 1;
    default: return 0;
  }
}


static int toby_intrinsic_type(int token, int allownothing)
{
  switch (token) {
    case TK_NUMBER:
    case TK_STRING:
    case TK_BOOLEAN:
    case TK_ARRAY:
      return 1;
    case TK_NOTHING:
      return allownothing ? 1 : 0;
    default: return 0;
  }
}


static void block (LexState *ls) {
  /* block -> chunk */
  FuncState *fs = ls->fs;
  BlockCnt bl;
  enterblock(fs, &bl, 0);
  chunk(ls, 0);
  lua_assert(bl.breaklist == NO_JUMP);
  leaveblock(fs);
}

static void assignment (LexState *ls, expdesc *v) {
  /* Toby form: var = expr */
  expdesc e;
  check_condition(ls, VLOCAL <= v->k && v->k <= VINDEXED, "syntax error");
  checknext(ls, '=');
  expr(ls, &e);
  luaK_setoneret(ls->fs, &e);  /* close last expression */
  luaK_storevar(ls->fs, v, &e);
}


static int cond (LexState *ls) {
  /* cond -> exp */
  expdesc v;
  expr(ls, &v);  /* read condition */
  if (v.k == VNIL) v.k = VFALSE;  /* `falses' are all equal here */
  luaK_goiftrue(ls->fs, &v);
  return v.f;
}


static void breakstat (LexState *ls) {
  FuncState *fs = ls->fs;
  BlockCnt *bl = fs->bl;
  int upval = 0;
  while (bl && !bl->isbreakable) {
    upval |= bl->upval;
    bl = bl->previous;
  }
  if (!bl)
    luaX_syntaxerror(ls, "no loop to break");
  if (upval)
    luaK_codeABC(fs, OP_CLOSE, bl->nactvar, 0, 0);
  luaK_concat(fs, &bl->breaklist, luaK_jump(fs));
}


static void whilestat (LexState *ls, int line) {
  /* whilestat -> WHILE cond DO block END */
  FuncState *fs = ls->fs;
  int whileinit;
  int condexit;
  BlockCnt bl;
  luaX_next(ls);  /* skip WHILE */
  whileinit = luaK_getlabel(fs);
  condexit = cond(ls);
  enterblock(fs, &bl, 1);
  block(ls);
  luaK_patchlist(fs, luaK_jump(fs), whileinit);
  check_match(ls, TK_ENDWHILE, TK_WHILE, line);
  leaveblock(fs);
  luaK_patchtohere(fs, condexit);  /* false conditions finish the loop */
}

static int exp1 (LexState *ls) {
  expdesc e;
  int k;
  expr(ls, &e);
  k = e.k;
  luaK_exp2nextreg(ls->fs, &e);
  return k;
}

static void forstat (LexState *ls, int line) {
  /* forstat -> FOR var = exp1 [TO|DOWNTO] exp1 [STEP exp1] block ENDFOR */
  FuncState *fs = ls->fs;
  BinOpr op;
  expdesc v;
  expdesc v1;
  expdesc v1copy;
  expdesc v1copy2;
  expdesc v2;
  int base = 0;
  int defaultstep = 1;
  int whileinit = 0;
  int condexit = 0;
  BlockCnt bl;

  luaX_next(ls);  /* skip `for' */

  singlevar(ls, &v);
  memcpy(&v1, &v, sizeof (expdesc));  /* save for later... */
  memcpy(&v1copy, &v1, sizeof (expdesc));  /* for when v1 gets mangled... */
  memcpy(&v1copy2, &v1, sizeof (expdesc));  /* and so on... */
  assignment(ls, &v);

  if (testnext(ls, TK_TO)) {
    defaultstep = 1;
  } else if (testnext(ls, TK_DOWNTO)) {
    defaultstep = -1;
  } else {
    luaX_syntaxerror(ls, LUA_QL("TO") " or " LUA_QL("DOWNTO") " expected");
  }

  base = fs->freereg;
  new_localvarliteral(ls, "(for limit)", 0);
  new_localvarliteral(ls, "(for step)", 1);

  exp1(ls);  /* limit */

  if (testnext(ls, TK_STEP))
    exp1(ls);  /* optional step */
  else {  /* default step = 1 (or -1 for downto) */
    luaK_codeABx(fs, OP_LOADK, fs->freereg, luaK_numberK(fs, defaultstep));
    luaK_reserveregs(fs, 1);
  }

  adjustlocalvars(ls, 2);  /* control variables */

  whileinit = luaK_getlabel(fs);

  op = ((defaultstep > 0) ? OPR_LE : OPR_GE);
  luaK_infix(fs, op, &v1);
  init_exp(&v2, VLOCAL, base);
  luaK_posfix(fs, op, &v1, &v2);
  luaK_goiftrue(fs, &v1);

  condexit = v1.f;

  enterblock(fs, &bl, 1);
  block(ls);
  check_match(ls, TK_ENDFOR, TK_FOR, line);
  op = OPR_ADD;
  luaK_infix(fs, OPR_ADD, &v1copy);
  init_exp(&v2, VLOCAL, base+1);  /* add step value to iterator variable. */
  luaK_posfix(fs, op, &v1copy, &v2);
  luaK_storevar(ls->fs, &v1copy2, &v1copy);

  luaK_patchlist(fs, luaK_jump(fs), whileinit);

  leaveblock(fs);
  luaK_patchtohere(fs, condexit);  /* false conditions finish the loop */
}


static int test_then_block (LexState *ls) {
  /* test_then_block -> [IF | ELSEIF] cond THEN block */
  int condexit;
  luaX_next(ls);  /* skip IF or ELSEIF */
  condexit = cond(ls);
  block(ls);  /* `then' part */
  return condexit;
}


static void ifstat (LexState *ls, int line) {
  /* ifstat -> IF cond THEN block {ELSEIF cond THEN block} [ELSE block] END */
  FuncState *fs = ls->fs;
  int flist;
  int escapelist = NO_JUMP;
  flist = test_then_block(ls);  /* IF cond THEN block */
  while (ls->t.token == TK_ELSEIF) {
    luaK_concat(fs, &escapelist, luaK_jump(fs));
    luaK_patchtohere(fs, flist);
    flist = test_then_block(ls);  /* ELSEIF cond THEN block */
  }
  if (ls->t.token == TK_ELSE) {
    luaK_concat(fs, &escapelist, luaK_jump(fs));
    luaK_patchtohere(fs, flist);
    luaX_next(ls);  /* skip ELSE (after patch, for correct line info) */
    block(ls);  /* `else' part */
  }
  else
    luaK_concat(fs, &escapelist, flist);
  luaK_patchtohere(fs, escapelist);
  check_match(ls, TK_ENDIF, TK_IF, line);
}


static void funcstat (LexState *ls, int line) {
  /* funcstat -> FUNCTION funcname body */
  expdesc v, b;

  /* No nested functions in Toby. */
  check_condition(ls, !ls->infunc, "function inside function");

  ls->infunc = 1;
  luaX_next(ls);  /* skip FUNCTION */
  lookupvar(ls, str_checkname(ls), &v, 1);
  lua_assert(v.k == VGLOBAL);  /* All functions are globals in Toby. */
  body(ls, &b, line);
  luaK_storevar(ls->fs, &v, &b);
  luaK_fixline(ls->fs, line);  /* definition `happens' in the first line */
  ls->infunc = 0;
}

static void default_var_value (LexState *ls, int token, expdesc *e) {
  /* Initialize variables with a sane default. */
  if (token == TK_NUMBER) {
    init_exp(e, VKNUM, 0);
    e->u.nval = 0;
  } else if (token == TK_BOOLEAN) {
    init_exp(e, VFALSE, 0);
  } else if (token == TK_STRING) {
    codestring(ls, e, luaX_newstring(ls, "", 0));
  } else {
    luaX_syntaxerror(ls, "data type expected");
  }
}

static int check_int_next(LexState *ls) {
    /* !!! FIXME: check for floating point... */
    int retval = (int) ls->t.seminfo.r;
    checknext(ls, TK_NUMBERLIT);
    return retval;
}

static void build_array_initializer2(LexState *ls, int datatype) {
  if (testnext(ls, '[')) {
    FuncState *fs = ls->fs;
    int base = fs->freereg;
    expdesc defval;
    expdesc var;
    BlockCnt bl1;
    BlockCnt bl2;
    int lo, hi;
    int prep, endfor;

    lo = check_int_next(ls);
    checknext(ls, TK_TO);
    hi = check_int_next(ls);
    checknext(ls, ']');
    /* !!! FIXME: change this error message. */
    check_condition(ls, (lo <= hi), "array is backwards");

    /* reserve a register for this dimension of the array... */
    enterblock(fs, &bl1, 1);  /* scope for declared variables */
    new_localvarliteral(ls, "(array register)", 0);

    /* Create an array in the register... */
    luaK_codeABC(fs, OP_NEWTABLE, base, (hi - lo) + 1, 0);
    luaK_reserveregs(fs, 1);
    adjustlocalvars(ls, 1);

    /*
     * Build for-loop, enter level.
     * Largely chopped from original Lua for loop parser... not sure how
     *  much of this is needed, but OP_FORLOOP expects the locals, etc.
     */
    new_localvarliteral(ls, "(for index)", 0);
    new_localvarliteral(ls, "(for limit)", 1);
    new_localvarliteral(ls, "(for step)", 2);
    new_localvarliteral(ls, "(for local)", 3);

    luaK_codeABx(fs, OP_LOADK, fs->freereg, luaK_numberK(fs, lo));
    luaK_reserveregs(fs, 1);
    luaK_codeABx(fs, OP_LOADK, fs->freereg, luaK_numberK(fs, hi));
    luaK_reserveregs(fs, 1);
    luaK_codeABx(fs, OP_LOADK, fs->freereg, luaK_numberK(fs, 1));
    luaK_reserveregs(fs, 1);
    adjustlocalvars(ls, 3);  /* control variables */

    prep = luaK_codeAsBx(fs, OP_FORPREP, base+1, NO_JUMP);
    enterblock(fs, &bl2, 0);  /* scope for declared variables */
    adjustlocalvars(ls, 1);
    luaK_reserveregs(fs, 1);

    init_exp(&var, VINDEXED, base);
    var.u.s.aux = base+4;

    enterlevel(ls);

    /* if token is TK_NAME, loop assigns default value to each index */
    if (ls->t.token == TK_NAME) {
      default_var_value(ls, datatype, &defval);
      luaK_storevar(fs, &var, &defval);
    } else {   /* otherwise, go deeper... */
      build_array_initializer2(ls, datatype);
      /* !!! FIXME: register (base+5) gets set in the recursion. Scary! */
      luaK_codeABC(fs, OP_SETTABLE, base, base+4, base+5);
    }

    leavelevel(ls);
    leaveblock(fs);  /* end of scope for declared variables */
    luaK_patchtohere(fs, prep);
    endfor = luaK_codeAsBx(fs, OP_FORLOOP, base+1, NO_JUMP);
    luaK_patchlist(fs, endfor, prep+1);

    leaveblock(fs);  /* end of scope for all of this. */
  }
}

static void build_array_initializer(LexState *ls, int datatype, expdesc *e) {
    check(ls, '[');
    build_array_initializer2(ls, datatype);
    /* !!! FIXME: This eats a free'd register! */
    /* !!! FIXME: for now, we'll reserve it, so luaK_storevar() can free it. */
    luaK_reserveregs(ls->fs, 1);
    init_exp(e, VLOCAL, ls->fs->freereg-1);
}

static void vardeclstat (LexState *ls, int global, int initialize) {
  /* stat -> INTRINSICTYPE NAME */
  int isarray = 0;
  expdesc v, e;
  int datatype = ls->t.token;
  luaX_next(ls);  /* skip data type. */

  if (datatype == TK_ARRAY) {
    isarray = 1;
    checknext(ls, TK_OF);
    datatype = ls->t.token;
    luaX_next(ls);

    if (initialize) {
      build_array_initializer(ls, datatype, &e);
    }
  }

  if (global) {
    lookupvar(ls, str_checkname(ls), &v, 1);
    lua_assert(v.k == VGLOBAL);
  } else {
    new_localvar(ls, str_checkname(ls), 0);
  }

  if (initialize) {
    if (!isarray) {
      if (testnext(ls, '=')) {
        expr(ls, &e);
      } else {
        default_var_value(ls, datatype, &e);
      }
    }
    if (global) {
      luaK_storevar(ls->fs, &v, &e);
    } else {
      adjust_assign(ls, 1, 1, &e);
    }
  }

  if (!global) {
    adjustlocalvars(ls, 1);
  }
}

#if 0
static void globalvarstat (LexState *ls) {
  expdesc v, e;
  int datatype = ls->t.token;
  luaX_next(ls);

  if (datatype == TK_ARRAY) {
    checknext(ls, TK_OF);
    datatype = ls->t.token;
    luaX_next(ls);
    build_array_initializer(ls, datatype, &e);
    lookupvar(ls, str_checkname(ls), &v, 1);
    lua_assert(v.k == VGLOBAL);
    luaK_storevar(ls->fs, &v, &e);
  } else {   /* scalar variable */
    lookupvar(ls, str_checkname(ls), &v, 1);
    lua_assert(v.k == VGLOBAL);
    if (ls->t.token == '=') {
      assignment(ls, &v);
    } else {
      default_var_value(ls, datatype, &e);
      luaK_storevar(ls->fs, &v, &e);
    }
  }
}
#endif

static void exprstat (LexState *ls) {
  /* stat -> func | assignment */
  FuncState *fs = ls->fs;
  expdesc v;
  primaryexp(ls, &v);
  if (v.k == VCALL)  /* stat -> func */
    SETARG_C(getcode(fs, &v), 1);  /* call statement uses no results */
  else {  /* stat -> assignment */
    assignment(ls, &v);
  }
}


static void retstat (LexState *ls) {
  /* stat -> RETURN explist */
  FuncState *fs = ls->fs;
  expdesc e;
  int first, nret;  /* registers with returned values */
  luaX_next(ls);  /* skip RETURN */
  if (block_follow(ls->t.token) || ls->t.token == ';')
    first = nret = 0;  /* return no values */
  else {
    nret = explist1(ls, &e);  /* optional return values */
    if (hasmultret(e.k)) {
      luaK_setmultret(fs, &e);
      if (e.k == VCALL && nret == 1) {  /* tail call? */
        SET_OPCODE(getcode(fs,&e), OP_TAILCALL);
        lua_assert(GETARG_A(getcode(fs,&e)) == fs->nactvar);
      }
      first = fs->nactvar;
      nret = LUA_MULTRET;  /* return all values */
    }
    else {
      if (nret == 1)  /* only one single value? */
        first = luaK_exp2anyreg(fs, &e);
      else {
        luaK_exp2nextreg(fs, &e);  /* values must go to the `stack' */
        first = fs->nactvar;  /* return all `active' values */
        lua_assert(nret == fs->freereg - first);
      }
    }
  }
  luaK_ret(fs, first, nret);
}


static int statement (LexState *ls) {
  int line = ls->linenumber;  /* may be needed for error messages */

  /*
   * Can't have code outside of a static function in Toby other than a
   *  function declaration. Mainline code goes into a function called "main",
   *  like in C.
   */
  if (!ls->infunc)
    check(ls, TK_FUNCTION);

  switch (ls->t.token) {
    case TK_IF: {  /* stat -> ifstat */
      ifstat(ls, line);
      return 0;
    }
    case TK_WHILE: {  /* stat -> whilestat */
      whilestat(ls, line);
      return 0;
    }
    case TK_FOR: {  /* stat -> forstat */
      forstat(ls, line);
      return 0;
    }
    case TK_FUNCTION: {
      funcstat(ls, line);  /* stat -> funcstat */
      return 0;
    }
    case TK_RETURN: {  /* stat -> retstat */
      retstat(ls);
      return 1;  /* must be last statement */
    }
    case TK_BREAK: {  /* stat -> breakstat */
      luaX_next(ls);  /* skip BREAK */
      breakstat(ls);
      return 1;  /* must be last statement */
    }
    default: {
      exprstat(ls);
      return 0;  /* to avoid warnings */
    }
  }
}


static void chunk (LexState *ls, int mainline) {
  /* chunk -> { stat [`;'] } */
  int islast = 0;
  enterlevel(ls);

  /* Toby requires global variable predeclaration at the start of the code. */
  if (mainline) {
    while (toby_intrinsic_type(ls->t.token, 0))
      vardeclstat(ls, 1, 1);
  }

  while (!islast && !block_follow(ls->t.token)) {
    islast = statement(ls);
    testnext(ls, ';');
    lua_assert(ls->fs->f->maxstacksize >= ls->fs->freereg &&
               ls->fs->freereg >= ls->fs->nactvar);
    ls->fs->freereg = ls->fs->nactvar;  /* free registers */
  }

  /* This is the mainline chunk, so tuck a call to user's main() at the end. */
  if (mainline) {
    /* largely cut-and-pasted from funcargs(). This could be cleaned up... */
    const int line = ls->linenumber;
    const char *fn = TOBY_MAINLINE_FNNAME;
    FuncState *fs = ls->fs;
    int base = 0;
    int nparams = 0;
    expdesc f;
    lookupvar(ls, luaX_newstring(ls, fn, strlen(fn)), &f, 0);
    luaK_exp2nextreg(fs, &f);
    base = f.u.s.info;  /* base register for call */
    nparams = fs->freereg - (base+1);
    init_exp(&f, VCALL, luaK_codeABC(fs, OP_CALL, base, nparams+1, 2));
    luaK_fixline(fs, line);
    ls->fs->freereg = ls->fs->nactvar;  /* free registers */
  }

  leavelevel(ls);
}

/* }====================================================================== */
