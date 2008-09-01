#ifndef H_QTABLE
#define H_QTABLE

#pragma warning( disable : 4522 )

extern "C" {
#include <LUA.h>
}

#include <math.h>
#include <stdlib.h>
#include <map>
#include <string>

using namespace std;

struct sValue;

class qTableCache{
protected:
	map<string,void *> _cache;
public:
	void * searchCache(const char * name);
	void updateCache(const char * name, void * data);
};

class qTable{
private:
	int ref_count;
protected:
	map<string,sValue> values;
	map<string,void *> _cache;
	char * _temp;
public:
	qTable();
	~qTable();
	sValue * getValueS(const char * name);
	sValue * getValue(const char * name, const char * tname =NULL);
	void setValueS(const char * name, sValue data);
	void setValue(const char * name, sValue data, const char * tname =NULL);

	sValue * getValuef(const char * fmt, ...);
	void setValuef(sValue & data, const char * fmt, ...);
	map<string,sValue> * getValues() {return &values;};

	const char * getParent(const char * name);
	const char * getSelf(const char * name);
/*
	qTable& operator=(qTable&){
		ref_count++;
		return *this;
	}
	*/
};

struct sValue {
private:
	int ref_count;
public:
	sValue(){
		reset();
	}
	~sValue(){
		if (ref_count>0){
			ref_count--;
		}else
			delete table;
	}
	void reset(){
		ref_count=0;
		pointer = NULL;
		table = NULL;
		type = LUA_TNIL;
		string = NULL;
		dword  = 0;
		number = 0.0;
		integer = 0;
		word = 0;
	}
	sValue(const char * val){
		reset();
		string = val;
		type = LUA_TSTRING;
	}
	sValue(int val){
		reset();
		integer = val;
		type = LUA_TNUMBER;
	}
	sValue(double val){
		reset();
		number = val;
		type = LUA_TNUMBER;
	}
	sValue(unsigned int val){
		reset();
		dword = val;
		type = LUA_TNUMBER;
	}
	sValue(unsigned short val){
		reset();
		word = val;
		type = LUA_TNUMBER;
	}
	sValue(const void * val){
		reset();
		pointer = val;
		type = LUA_TUSERDATA;
	}
	sValue(qTable * val){
		reset();
		table = val;
		type = LUA_TTABLE;
	}
	sValue& operator=(sValue& a){
		//__SCRIPT->dprintf("sValue copy reference");
		this->dword = a.dword;
		this->integer = a.integer;
		this->pointer = a.pointer;
		this->string = a.string;
		this->table = a.table;
		this->number = a.number;
		this->type = a.type;
		this->word = a.word;
		ref_count++;
		return *this;
	}

	int toInt(){
		return atoi(string);
	}

	double toDouble(){
		return atof(string);
	}

	int guessType(){
		if (string){
			return LUA_TSTRING;
		}else if (number||integer){
			return LUA_TNUMBER;
		}else if (pointer){
			return LUA_TUSERDATA;
		}else{
			return LUA_TNUMBER;
		}
	}

	const char * string;
	int integer;
	unsigned int dword;
	unsigned short word;
	const void * pointer;
	double number;
	qTable * table;
	int type;
};

#endif