/*
** $Id: llex.h,v 1.58 2006/03/23 18:23:32 roberto Exp $
** Lexical Analyzer
** See Copyright Notice in lua.h
*/

#ifndef tobylex_h
#define tobylex_h

#include "lobject.h"
#include "lzio.h"


#define FIRST_RESERVED	257

/* maximum length of a reserved word */
#define TOKEN_LEN	(sizeof("function")/sizeof(char))


/*
* WARNING: if you change the order of this enumeration,
* grep "ORDER RESERVED"
*/
enum RESERVED {
  /* terminal symbols denoted by reserved words */
  TK_AND = FIRST_RESERVED, TK_BREAK,
  TK_ELSE, TK_ELSEIF, TK_ENDFUNCTION, TK_ENDIF, TK_ENDFOR, TK_ENDWHILE,
  TK_FALSE, TK_FOR, TK_FUNCTION,
  TK_IF, TK_TO, TK_DOWNTO, TK_STEP, TK_OF, TK_IN, TK_LOCAL, TK_NOT, TK_OR,
  TK_NUMBER, TK_STRING, TK_BOOLEAN, TK_ARRAY, TK_NOTHING,
  TK_RETURN, TK_RETURNS, TK_TRUE, TK_UNTIL, TK_WHILE,
  /* DON'T ADD RESERVED KEYWORDS AFTER TK_WHILE! */
  /* other terminal symbols */
  TK_CONCAT, TK_DOTS, TK_EQ, TK_GE, TK_LE, TK_NE, TK_NUMBERLIT,
  TK_NAME, TK_STRINGLIT, TK_EOL, TK_EOS
};

#define TOBY_MAINLINE_FNNAME "main"

/* number of reserved words */
#define NUM_RESERVED	(cast(int, TK_WHILE-FIRST_RESERVED+1))


/* array with token `names' */
LUAI_DATA const char *const luaX_tokens [];


typedef union {
  lua_Number r;
  TString *ts;
} SemInfo;  /* semantics information */


typedef struct Token {
  int token;
  SemInfo seminfo;
} Token;


typedef struct LexState {
  int current;  /* current character (charint) */
  int linenumber;  /* input line counter */
  int lastline;  /* line of last token `consumed' */
  Token t;  /* current token */
  Token lookahead;  /* look ahead token */
  struct FuncState *fs;  /* `FuncState' is private to the parser */
  struct lua_State *L;
  ZIO *z;  /* input stream */
  Mbuffer *buff;  /* buffer for tokens */
  TString *source;  /* current source name */
  char decpoint;  /* locale decimal point */
  int infunc; /* non-zero if we're parsing a function body, 0 if we're not. */
  int to_resolve_table;  /* pos of table of to-be-resolved on Lua stack. */
  int resolved_table;  /* pos of table of resolved on Lua stack. */
} LexState;


LUAI_FUNC void luaX_init (lua_State *L);
LUAI_FUNC void luaX_setinput (lua_State *L, LexState *ls, ZIO *z,
                              TString *source);
LUAI_FUNC TString *luaX_newstring (LexState *ls, const char *str, size_t l);
LUAI_FUNC void luaX_next (LexState *ls);
LUAI_FUNC void luaX_lookahead (LexState *ls);
LUAI_FUNC void luaX_lexerror (LexState *ls, const char *msg, int token);
LUAI_FUNC void luaX_syntaxerror (LexState *ls, const char *s);
LUAI_FUNC const char *luaX_token2str (LexState *ls, int token);

#endif
