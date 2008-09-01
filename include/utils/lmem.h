#ifndef H_LMEM
#define H_LMEM

extern "C" {
#include <LUA.h>
#include <lualib.h>
#include <lauxlib.h>
}
#include <hash_map>
#include <string>
using namespace std;
using namespace stdext;
#include "qDebug.h"

//struct qLDatap;
struct qLdata;

#define qLDatap hash_map<string,qLdata>

struct qLdata{
	lua_Number	_number;
	const char * _string;
	qLDatap * _table;
	int type;
};

extern qLDatap * LuaData;

class qLMem: public qDebug{
public:
	qLMem();
	~qLMem();
	void prepareFunctions(lua_State * Lua);
	static int mem_new(lua_State * Lua);
	static qLDatap * mem_check(lua_State * Lua, int index);
	static int mem_gc(lua_State * Lua);
	static int mem_set(lua_State * Lua);
	static int mem_get(lua_State * Lua);
};

const qLdata * exists(qLDatap * data,const char * name);

#endif