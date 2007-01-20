/*
** $Id: linit.c,v 1.14 2005/12/29 15:32:11 roberto Exp $
** Initialization of libraries for lua.c
** See Copyright Notice in lua.h
*/


#define linit_c
#define LUA_LIB

#include "lua.h"

#include "lualib.h"
#include "lauxlib.h"

/* !!! FIXME: temporary hack. */
#define LUAHOOK(sym) \
    static int luahook_##sym(lua_State *L) { \
        printf("\t%s()\n", #sym); \
        return 0; \
    }

#define LUAHOOK_RETNUM(sym) \
    static int luahook_##sym(lua_State *L) { \
        printf("\t%s()\n", #sym); \
        lua_pushnumber(L, 0.0f); \
        return 1; \
    }

#define LUAHOOK_NUM(sym) \
    static int luahook_##sym(lua_State *L) { \
        const lua_Number n1 = luaL_checknumber(L, 1); \
        printf("\t%s(%f)\n", #sym, (float) n1); \
        return 0; \
    }

#define LUAHOOK_STR(sym) \
    static int luahook_##sym(lua_State *L) { \
        const char *s1 = luaL_checkstring(L, 1); \
        printf("\t%s(\"%s\")\n", #sym, s1); \
        return 0; \
    }

#define LUAHOOK_NUM_RETNUM(sym) \
    static int luahook_##sym(lua_State *L) { \
        const lua_Number n1 = luaL_checknumber(L, 1); \
        printf("\t%s(%f)\n", #sym, (float) n1); \
        lua_pushnumber(L, 0.0f); \
        return 1; \
    }

#define LUAHOOK_NUM_NUM(sym) \
    static int luahook_##sym(lua_State *L) { \
        const lua_Number n1 = luaL_checknumber(L, 1); \
        const lua_Number n2 = luaL_checknumber(L, 2); \
        printf("\t%s(%f, %f)\n", #sym, (float) n1, (float) n2); \
        return 0; \
    }

#define LUAHOOK_NUM_NUM_NUM(sym) \
    static int luahook_##sym(lua_State *L) { \
        const lua_Number n1 = luaL_checknumber(L, 1); \
        const lua_Number n2 = luaL_checknumber(L, 2); \
        const lua_Number n3 = luaL_checknumber(L, 3); \
        printf("\t%s(%f, %f, %f)\n", #sym, (float) n1, (float) n2, (float) n3); \
        return 0; \
    }

LUAHOOK(showturtle)
LUAHOOK(hideturtle)
LUAHOOK(hometurtle)
LUAHOOK(enablefence)
LUAHOOK(disablefence)
LUAHOOK_STR(drawstring)
LUAHOOK_RETNUM(random)
LUAHOOK_RETNUM(getturtlex)
LUAHOOK_RETNUM(getturtley)
LUAHOOK_RETNUM(getturtlespacewidth)
LUAHOOK_RETNUM(getturtlespaceheight)
LUAHOOK(cleanupturtlespace)
LUAHOOK(setpenup)
LUAHOOK(setpendown)
LUAHOOK(addturtle)
LUAHOOK_NUM_NUM_NUM(setpencolorrgb)
LUAHOOK_NUM(useturtle)
LUAHOOK_NUM_RETNUM(round)
LUAHOOK_NUM_RETNUM(stringlength)
LUAHOOK_NUM(pause)
LUAHOOK_NUM(setangle)
LUAHOOK_NUM(goforward)
LUAHOOK_NUM(gobackward)
LUAHOOK_NUM(turnright)
LUAHOOK_NUM(turnleft)
LUAHOOK_NUM(setpencolor)
LUAHOOK_NUM_NUM(setturtlexy)

static void add_toby_functions(lua_State *L)
{
    #define SET_LUAHOOK(sym) \
        lua_pushcfunction(L, luahook_##sym); \
        lua_setglobal(L, #sym);
    SET_LUAHOOK(showturtle)
    SET_LUAHOOK(hideturtle)
    SET_LUAHOOK(hometurtle)
    SET_LUAHOOK(enablefence)
    SET_LUAHOOK(disablefence)
    SET_LUAHOOK(drawstring)
    SET_LUAHOOK(random)
    SET_LUAHOOK(getturtlespacewidth)
    SET_LUAHOOK(getturtlespaceheight)
    SET_LUAHOOK(getturtlex)
    SET_LUAHOOK(getturtley)
    SET_LUAHOOK(cleanupturtlespace)
    SET_LUAHOOK(setpencolorrgb)
    SET_LUAHOOK(setpenup)
    SET_LUAHOOK(setpendown)
    SET_LUAHOOK(addturtle)
    SET_LUAHOOK(useturtle)
    SET_LUAHOOK(round)
    SET_LUAHOOK(stringlength)
    SET_LUAHOOK(pause)
    SET_LUAHOOK(setangle)
    SET_LUAHOOK(goforward);
    SET_LUAHOOK(gobackward);
    SET_LUAHOOK(turnright);
    SET_LUAHOOK(turnleft);
    SET_LUAHOOK(setpencolor);
    SET_LUAHOOK(setturtlexy);
    #undef SET_LUAHOOK
}



static const luaL_Reg lualibs[] = {
  {"", luaopen_base},
  {LUA_LOADLIBNAME, luaopen_package},
  {LUA_TABLIBNAME, luaopen_table},
  {LUA_IOLIBNAME, luaopen_io},
  {LUA_OSLIBNAME, luaopen_os},
  {LUA_STRLIBNAME, luaopen_string},
  {LUA_MATHLIBNAME, luaopen_math},
  {LUA_DBLIBNAME, luaopen_debug},
  {NULL, NULL}
};


LUALIB_API void luaL_openlibs (lua_State *L) {
  const luaL_Reg *lib = lualibs;
  for (; lib->func; lib++) {
    lua_pushcfunction(L, lib->func);
    lua_pushstring(L, lib->name);
    lua_call(L, 1, 0);
  }
  add_toby_functions(L);
}

