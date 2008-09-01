#include "modules/script.h"
#include <malloc.h>
#include <string.h>
#include <stdio.h>

extern "C" {
#include <LUA.h>
#include <lauxlib.h>
#include <lualib.h>
}
#include "modules/game.h"
#include "modules/renderer.h"
#include "modules/qTexturer.h"
#include "modules/controler.h"

#include "base.h"
#include "common.h"

cBase * __parent;
static SDL_mutex * _mutex;

void cScript::l_setKey(int key, int state){
	lockMutex();
		lua_getglobal(Lua, "setKey");
		lua_pushinteger(Lua,key);
		lua_pushinteger(Lua,state);
		_call(Lua,2,0,0);
	unlockMutex();
}

void cScript::l_setMouseButton(unsigned short button, unsigned short button2){
	lockMutex();
		lua_getglobal(Lua, "setMouseButton");
		lua_pushinteger(Lua,button);
		lua_pushinteger(Lua,button2);
		_call(Lua,2,0,0);
	unlockMutex();
}

void cScript::l_setMousePos(int x, int y, int rx, int ry){
	lockMutex();
	lua_getglobal(Lua, "setMousePos");
	lua_pushinteger(Lua,x);
	lua_pushinteger(Lua,y);
	lua_pushinteger(Lua,rx);
	lua_pushinteger(Lua,ry);
	_call(Lua,4,0,0);
	unlockMutex();
}

float * getArrayf(lua_State * L, int index){
	int count = luaL_getn(L, index);
	if (lua_istable(L,index)){
		float * _array = (float*) malloc(sizeof(float)*count);
		if (_array){
			for (int i=0; i < count; i++){
				lua_rawgeti(L,index,i+1);
				_array[i] = (float)lua_tonumber(L,-1);
			}
		}
		return _array;
	}
	return NULL;
}

double * getArrayd(lua_State * L, int index){
	int count = luaL_getn(L, index);
	double * _array = new double[count];
	for (int i=0; i < count; i++){
		lua_rawgeti(L,index,i+1);
		_array[i] = (double)lua_tonumber(L,-1);
	}
	return _array;
}

const char ** getArrays(lua_State * L, int index){
	int count = luaL_getn(L, index);
	if (lua_istable(L,index)){
		const char ** _array = (const char**) malloc(sizeof(char*)*count);
		if (_array){
			for (int i=0; i < count; i++){
				lua_rawgeti(L,index,i+1);
				_array[i] = (const char*)lua_tostring(L,-1);
			}
		}
		return _array;
	}
	return NULL;
}

/*
	types:
		0	-	integer
		1	-	string
		2	-	float
*/
static int l_and(lua_State *L){
	uint a = lua_tointeger(L,1);
	uint b = lua_tointeger(L,2);
	lua_pushnumber(L,a&b);
	return 1;
}

static int l_or(lua_State *L){
	uint a = lua_tointeger(L,1);
	uint b = lua_tointeger(L,2);
	lua_pushnumber(L,a|b);
	return 1;
}

static int l_xor(lua_State *L){
	uint a = lua_tointeger(L,1);
	uint b = lua_tointeger(L,2);
	lua_pushnumber(L,a^b);
	return 1;
}

static int l_not(lua_State *L){
	int a = lua_tointeger(L,1);
	lua_pushinteger(L,(lua_Integer)~a);
	return 1;
}

static int l_shl(lua_State *L){
	uint a = lua_tointeger(L,1);
	uint b = lua_tointeger(L,2);
	lua_pushinteger(L,a << b);
	return 1;
}

static int l_shr(lua_State *L){
	uint a = lua_tointeger(L,1);
	uint b = lua_tointeger(L,2);
	lua_pushinteger(L,a >> b);
	return 1;
}

void cScript::lockMutex(){
	SDL_mutexP(_mutex);
}

void cScript::unlockMutex(){
	SDL_mutexV(_mutex);
}

cValue cScript::_getField(lua_State * L, const char * name, int type){
	lockMutex();
	cValue value={NULL};

	lua_pushstring(L, name);
	lua_gettable(L,-2);
	if (lua_isnoneornil(L,-1)){
			__SCRIPT->dprintf("field \"%s\" doesn't exist!\n",name);
			value.string = NULL;
	}else{
		switch (type){
		case 0:
			if (lua_isnumber(L, -1)){
				value.integer=(int)lua_tointeger(L,-1);
			}else{
				__SCRIPT->dprintf("\"%s\" is not an integer\n",name);
				value.integer = 0;
			}
			break;
		case 1:
			if (lua_isstring(L, -1)){
				value.string=lua_tostring(L,-1);
			}else{
				__SCRIPT->dprintf("\"%s\" is not a string\n",name);
				value.string = NULL;
			}
			break;
		case 2:
			if (lua_isnumber(L, -1)){
				value.number = lua_tonumber(L,-1);
			}else{
				__SCRIPT->dprintf("\"%s\" is not a float\n",name);
				value.number = 0;
			}
		}
	}
	lua_pop(L,1);
	unlockMutex();
	return value;
}

void cScript::setFieldValue(lua_State * L, const char * name, int value){
	lockMutex();
	lua_pushstring(L,name);
	lua_pushinteger(L,value);
	lua_settable(L,-3);
	unlockMutex();
}

void cScript::setFieldValue(lua_State * L, const char * name, float value){
	lockMutex();
	lua_pushstring(L,name);
	lua_pushnumber(L,value);
	lua_settable(L,-3);
	unlockMutex();
}

void cScript::setFieldValue(lua_State * L, const char * name, const char * value){
	lockMutex();
	lua_pushstring(L,name);
	lua_pushstring(L,value);
	lua_settable(L,-3);
	unlockMutex();
}

int cScript::getFieldi(lua_State * L, const char * name){
	return _getField(L,name,0).integer;
}
const char * cScript::getFields(lua_State * L, const char * name){
	return _getField(L,name,1).string;
}
double cScript::getFieldf(lua_State * L, const char * name){
	return _getField(L,name,2).number;
}
	
cScript::cScript(cBase * parent): cModule(parent){
	__parent = parent;
	setName("cScript");
	scriptname = new char[512];

	tmp = (char*)malloc(256);
	Lua = lua_open();   /* opens Lua */

	//lua_pushcfunction(lua_CFunction)
	_mutex = SDL_CreateMutex();
	luaL_openlibs(Lua);
/*
	luaopen_base(Lua);		// opens the basic library 
	luaopen_table(Lua);		// opens the table library
	luaopen_io(Lua);		// opens the I/O library
	luaopen_package(Lua);	// opens the package library
	luaopen_string(Lua);	// opens the string lib.
	luaopen_math(Lua);		// opens the math lib.
	*/
#ifdef DEBUGGING
	dprintf("cScript()\n");
#endif

}

cScript::~cScript(){
	SDL_DestroyMutex(_mutex);
	lua_close(Lua);
	free(tmp);
	delete scriptname;
}

void cScript::_call(lua_State *L,int a, int b,int c){
	int ret;
	if ((ret=lua_pcall(L, a, b, c)) != 0 ){
		switch (ret){
			case LUA_ERRRUN:
				dprintf("Runtime error: %s\n",lua_tostring(L,-1));
				break;
			case LUA_ERRMEM:
				dprintf("Memalloc error: %s\n",lua_tostring(L,-1));
				break;
			case LUA_ERRERR:
				dprintf("Error handler error: %s\n",lua_tostring(L,-1));
				break;
			default:
				dprintf("Unknown error code %d: %s\n",ret,lua_tostring(L,-1));
		}
		lua_pop(L,1);
		GAME->glogic->quit();
	}
}

void cScript::call(const char * name){
	try {
		if (name){
			lockMutex();
			lua_getglobal(Lua, name);
			_call(Lua);
			unlockMutex();
		}
	}DEFAULT_EXCEPTION(stderr)
}

void cScript::call(const char * name, int p1){
	try {
		if (name){
			lockMutex();
			lua_getglobal(Lua, name);
			lua_pushnumber(Lua,p1);
			_call(Lua,1);
			unlockMutex();
		}
	}DEFAULT_EXCEPTION(stderr)
}

void cScript::call(const char * object, const char * function){
	try {
		if (object && function){
			lockMutex();
			lua_getglobal(Lua, "callObject");
			lua_pushstring(Lua,object);
			lua_pushstring(Lua,function);
			_call(Lua,2);
			unlockMutex();
		}else{
			dprintf("Tried to call object function with NULL parameter");
		}
	}DEFAULT_EXCEPTION(stderr)
}

void cScript::call(const char * name, float x, float y, float z, float size, float r, float g, float b, float a){
try {
		if (name){
			lockMutex();
			lua_getglobal(Lua,name);
			lua_pushnumber(Lua,x);
			lua_pushnumber(Lua,y);
			lua_pushnumber(Lua,z);
			lua_pushnumber(Lua,size);
			lua_pushnumber(Lua,r);
			lua_pushnumber(Lua,g);
			lua_pushnumber(Lua,b);
			lua_pushnumber(Lua,a);
			_call(Lua,8);
			unlockMutex();
		}else{
			dprintf("Tried to call object function with NULL parameter");
		}
	}DEFAULT_EXCEPTION(stderr)	
}

int cScript::call(const char * fname, char * p1, int p2){
	int ret=0;
	lockMutex();
	lua_getglobal(Lua,fname);
	lua_pushstring(Lua,p1);
	lua_pushnumber(Lua,p2);
	_call(Lua, 2, 1);
	ret = (int)lua_tointeger(Lua,-1);
	lua_pop(Lua, 1);
	unlockMutex();
	return ret;
}

void cScript::callLF(int self, const char * fmt,...){
	lockMutex();
	char text[1024];
	memset(text,0,1024);
	
	va_list ap;
	if (fmt==NULL) return;
	va_start(ap,fmt);
	vsprintf(text,fmt,ap);
	va_end(ap);
	callLF(text,self);
	unlockMutex();
}

void cScript::callLF(const char * name, int self){
	lockMutex();
	const void * ptr = SCRIPT->mainTable.getValue(name)->pointer;
	//get class self pointer
	const void * self1 = SCRIPT->mainTable.getValue(SCRIPT->mainTable.getSelf(name))->pointer;
	void * ptr2 = (void*) ptr;
	lua_rawgeti(Lua, LUA_REGISTRYINDEX, (int)ptr2);
	//use class self pointer if present
	if (self) lua_rawgeti(Lua, LUA_REGISTRYINDEX, (int)self1);
	_call(Lua, 1, 0);
	unlockMutex();
}

void cScript::callLF(int index, int self){
	lockMutex();
	lua_rawgeti(Lua, LUA_REGISTRYINDEX, index);
	//use class self pointer if present
	if (self) lua_rawgeti(Lua, LUA_REGISTRYINDEX, (int)self);
	_call(Lua, 1, 0);
	unlockMutex();
}

int cScript::callLF(int index, int self, int param1, int param2, int param3){
	lockMutex();
	lua_rawgeti(Lua, LUA_REGISTRYINDEX, index);
	//use class self pointer if present
	if (self) lua_rawgeti(Lua, LUA_REGISTRYINDEX, (int)self);
	lua_pushinteger(Lua,param1);
	lua_pushinteger(Lua,param2);
	lua_pushinteger(Lua,param3);
	_call(Lua, 4, 1);
	int ret = (int)lua_tointeger(Lua,-1);
	lua_pop(Lua, 1);
	unlockMutex();
	return ret;
}

cBase * cScript::getParent(lua_State *L){
/*	lua_getglobal (L, "__parent");
	void * ptr = lua_touserdata (L, -1);
	lua_pop(L, 1);
	*/
	return 0;
}
/*
int cScript::l_init_windows(lua_State * L){
	__RENDER->initWindows();
	return 0;
}
*/

void cScript::updateData(){
	lockMutex();
	qSphere * obj = __GAME->glogic->curObject();
	qVector pos = obj->getPosition();
	qVector angle = obj->getAngle();
	int mx = CONTROL->getMouse()->getPos().x;
	int my = CONTROL->getMouse()->getPos().y;
	int rmx = CONTROL->getMouse()->getRelPos().x;
	int rmy = CONTROL->getMouse()->getRelPos().y;
	int buttons = CONTROL->getMouse()->getButtons();
	int buttons2 = CONTROL->getMouse()->getPrevButtons();

	lua_getglobal(Lua, "data");
	lua_pushstring(Lua, "camera");
	lua_gettable(Lua,-2);
		lua_pushstring(Lua, "pos");
		lua_gettable(Lua,-2);
			setFieldValue(Lua,"x",pos.x);
			setFieldValue(Lua,"y",pos.y);
			setFieldValue(Lua,"z",pos.z);
		lua_pop(Lua,1);
		lua_pushstring(Lua, "angle");
		lua_gettable(Lua,-2);
			setFieldValue(Lua,"x",angle.x);
			setFieldValue(Lua,"y",angle.y);
			setFieldValue(Lua,"z",angle.z);
		lua_pop(Lua,1);
	lua_pop(Lua,1);
	unlockMutex();
}

void cScript::init(){
	strcpy(scriptname,"scripts/init.lua");
	status  = TS_RUNNING;
}

void cScript::run(){
	int error;
	lockMutex();
	lua_register(Lua,"bAnd",l_and);
	lua_register(Lua,"bOr",l_or);
	lua_register(Lua,"bXor",l_xor);
	lua_register(Lua,"bNot",l_not);
	lua_register(Lua,"bShl",l_shl);
	lua_register(Lua,"bShr",l_shr);

	gl.prepareFunctions(Lua);
	engine.prepareFunctions(Lua);
	mem.prepareFunctions(Lua);

	//error = luaL_loadfile(Lua,scriptname) || lua_pcall(Lua, 0, 0, 0);
	error = luaL_dofile(Lua,scriptname);// || lua_pcall(Lua, 0, 0, 0);
	unlockMutex();

	if (error) {
		dprintf("%s\n", lua_tostring(Lua, -1));
		lua_pop(Lua, 1);
	}
	status |= TS_RUNNING2;
}

void cScript::stop(){
	status  = 0;
}
