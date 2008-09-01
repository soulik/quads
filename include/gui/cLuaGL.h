#ifndef H_CLUAGL
#define H_CLUAGL

extern "C" {
#include <LUA.h>
#include <lualib.h>
#include <lauxlib.h>
}
struct gl_str_value {
  const char *str;
  unsigned int value;
};

class cLuaGL{
public:
	void prepareFunctions(lua_State * Lua);
};

#endif