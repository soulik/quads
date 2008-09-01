#ifndef H_ENGINE
#define H_ENGINE

extern "C" {
#include <LUA.h>
#include <lualib.h>
#include <lauxlib.h>
}

class qEngine {
public:
	void prepareFunctions(lua_State * Lua);
};

#endif