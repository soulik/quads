#ifndef H_QSCRIPT
#define H_QSCRIPT

extern "C" {
#include <LUA.h>
}

#include "module.h"
#include "qTable.h"
#include "gui/cLuaGL.h"
#include "utils/lmem.h"
#include "engine/engine.h"

extern cBase * __parent;

union cValue{
	int integer;
	const char * string;
	double number;
};

class cScript: public cModule{
private:
	cLuaGL gl;
	qEngine engine;
	qLMem mem;
protected:
	lua_State *Lua;
	char * scriptname;
	cBase * getParent(lua_State *L);
	void _call(lua_State *L,int a=0, int b=0,int c=0);
public:
	cScript(cBase * parent);
	~cScript();
	void init();
	void run();
	void stop();

	//special call functions
	void call(const char * name);
	void call(const char * name, int p1);
	int call(const char * fname, char * p1, int p2);
	void call(const char * object, const char * function);
	void call(const char * name, float x, float y, float z, float size, float r, float g, float b, float a);
	void callLF(int self, const char * fmt,...);
	void callLF(const char * name, int self=0);
	void callLF(int index, int self=0);
	int callLF(int index, int self, int param1, int param2, int param3);
	void l_setKey(int key, int state);
	void l_setMouseButton(unsigned short button, unsigned short button2);
	void l_setMousePos(int x, int y, int rx, int ry);
	void updateData();

	//data management
	static int getFieldi(lua_State * L, const char * name);
	static const char * getFields(lua_State * L, const char * name);
	static double getFieldf(lua_State * L, const char * name);
	static cValue _getField(lua_State * L, const char * name, int type);

	static void setFieldValue(lua_State * L, const char * name, int value);
	static void setFieldValue(lua_State * L, const char * name, float value);
	static void setFieldValue(lua_State * L, const char * name, const char * value);

	//mutex management
	static void lockMutex();
	static void unlockMutex();
	char * tmp;
	qTable mainTable;
};

float * getArrayf(lua_State * L, int index);
double * getArrayd(lua_State * L, int index);
const char ** getArrays(lua_State * L, int index);

#endif