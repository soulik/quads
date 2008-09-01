#include "utils/lmem.h"
#include <stdio.h>

qLDatap * LuaData = NULL;

qLMem::qLMem(){
	setName("mem");
}

qLMem::~qLMem(){
}

qLDatap * qLMem::mem_check(lua_State * L, int index){
	qLDatap * userdata = (qLDatap*)luaL_checkudata(L,index,"qLMem");
	return (qLDatap*)userdata;
}

const qLdata * exists(qLDatap * data,const char * name){
	if (data){
		qLDatap::const_iterator iter = data->find(name);
		if (iter != data->end()){
			return &(iter->second);
		}
	}
	return NULL;
}

int qLMem::mem_new(lua_State * L){
	qLDatap * data = new hash_map<string,qLdata>;
	int param = lua_tointeger(L, 1);
	
	if (param){
		if (LuaData){
			delete LuaData;
		}
		LuaData = data;
	}
	lua_pushlightuserdata(L,data);
	
	luaL_getmetatable(L, "qLMem");
	lua_setmetatable(L, -2);
	return 1;
}

int qLMem::mem_gc(lua_State * L){
	qLDatap * self = mem_check(L,1);
	delete self;
	//printf("mem:gc()\n");
	return 0;
}

int qLMem::mem_set(lua_State * L){
	int ret = 0;
	qLDatap * self = mem_check(L,1);
	const char * key = lua_tostring(L,2);
	int type = lua_type(L,3);
	if (key){
		(*self)[key].type = type;
		//printf("mem:set(%s)=",key);
		switch (type){
			case LUA_TBOOLEAN:
			case LUA_TNUMBER:
				(*self)[key]._number = lua_tonumber(L,3);
				//printf("%f\n",(*self)[key].number);
				break;
			case LUA_TSTRING:
				(*self)[key]._string = lua_tostring(L,3);
				//printf("%s\n",(*self)[key].string);
				break;
			case LUA_TNIL:
			case LUA_TFUNCTION:
			case LUA_TTHREAD:
			case LUA_TTABLE:
				(*self)[key]._table = (qLDatap*)lua_topointer(L,3);
				lua_pushlightuserdata(L,(void*)(*self)[key]._table);
				lua_pushvalue(L,3);
				lua_settable(L, LUA_REGISTRYINDEX);					
				break;
			default:
				(*self)[key]._table = (qLDatap*)lua_topointer(L,3);
				lua_pushvalue(L,3);
				ret = lua_getmetatable(L,1);
				lua_settable(L, LUA_REGISTRYINDEX);					
				break;
		}
	}
	return 0;
}
int qLMem::mem_get(lua_State * L){
	int ret = 0;
	qLDatap * self = mem_check(L,1);
	const char * key = lua_tostring(L,2);
	if (key){
		switch ((*self)[key].type){
			case LUA_TBOOLEAN:
				lua_pushboolean(L,(int)(*self)[key]._number);
			case LUA_TNUMBER:
				lua_pushnumber(L,(*self)[key]._number);
				break;
			case LUA_TSTRING:
				lua_pushstring(L,(*self)[key]._string);
				break;
			case LUA_TNIL:
			case LUA_TFUNCTION:
			case LUA_TTHREAD:
			case LUA_TTABLE:
				lua_pushlightuserdata(L,(void*)(*self)[key]._table);
				lua_gettable(L, LUA_REGISTRYINDEX);
				break;
			default:
				lua_pushlightuserdata(L,(void*)(*self)[key]._table);
				lua_pushvalue(L,-1);
				lua_gettable(L, LUA_REGISTRYINDEX);
				ret = lua_setmetatable(L,-2);
				break;
		}
	}
	//printf("mem:get(%s)\n",key);
	return 1;
}

static const luaL_reg luaMem[] = {
	{"__gc",qLMem::mem_gc},
	{"__newindex",qLMem::mem_set},
	{"__index",qLMem::mem_get},
	{NULL,NULL}
};
static const luaL_reg luaMemc[] = {
	{"new",qLMem::mem_new},
	{NULL,NULL}
};

void qLMem::prepareFunctions(lua_State *Lua){
	luaL_newmetatable(Lua, "qLMem");
	luaL_openlib(Lua, NULL, luaMem, 0);
	luaL_openlib(Lua, "mem", luaMemc, 0);
}