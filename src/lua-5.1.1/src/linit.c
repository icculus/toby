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
#define LUAHOOK_INT(sym) \
    static int luahook_##sym(lua_State *L) { \
        const int i1 = luaL_checkinteger(L, 1); \
        printf("\t%s(%d)\n", #sym, i1); \
        return 0; \
    }

LUAHOOK_INT(goForward)
LUAHOOK_INT(turnRight)
LUAHOOK_INT(turnLeft)
LUAHOOK_INT(setPenColor)

static void add_toby_functions(lua_State *L)
{
    #define SET_LUAHOOK(sym) \
        lua_pushcfunction(L, luahook_##sym); \
        lua_setglobal(L, #sym);
    SET_LUAHOOK(goForward);
    SET_LUAHOOK(turnRight);
    SET_LUAHOOK(turnLeft);
    SET_LUAHOOK(setPenColor);
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

