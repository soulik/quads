#include "qTable.h"
#include <string.h>
#include "modules/script.h"
#include "common.h"

//major speedup with cache - about +100 fps ;)
qTableCache tcache;

qTable::qTable(){
	_temp = NULL;
	_temp=(char*)malloc(1024);
	ref_count=0;
}

qTable::~qTable(){
	if (ref_count>0){
		ref_count--;
	}else{
		if (_temp) free(_temp);
	}
}

/*
	takes N - time in the worst case
	map is not sorted - BTREE (name letters are forks & leafs) would be better
*/
void * qTableCache::searchCache(const char * name){
	map<string,void *>::iterator temp;
	temp = _cache.find(name);
	if( temp == _cache.end() ) {
		return NULL;
	}
	return temp->second;
}

void qTableCache::updateCache(const char * name, void * data){
	_cache.insert(make_pair(name,data));
}

sValue * qTable::getValuef(const char * fmt, ...){
	char text[1024];
	memset(text,0,1024);
	
	va_list ap;
	if (fmt==NULL) return NULL;
	va_start(ap,fmt);
	vsprintf(text,fmt,ap);
	va_end(ap);
	return getValue(text);
}
sValue * qTable::getValueS(const char * name){
	return &(values[name]);
}

sValue * qTable::getValue(const char * name, const char * tname){
	static const char * bname = NULL;
	void * sc=NULL;
	if (!tname){
		sc = tcache.searchCache(name);
		tname = name;
	}

	if (!sc){
		char * _name = _strdup(name);
		const char deliminator[]="::";
		char * result;
		map<string,sValue>::iterator temp;

		result = strstr(_name,deliminator);
		//there is a deliminator in path
		if (result){
			//remove a deliminator from the begining of the result
			result+=2;
			strtok(_name,deliminator);
			
			//let's check if this table exists
			temp = values.find(_name);
			//if not, return empty sValue
			if( temp == values.end() ) {
				return NULL;
			}
			//proceed...
			sValue * ret = values[_name].table->getValue(result,tname);
			free(_name);
			return ret;
		}else{
			//no further tables are needed so we can read a value here
			tcache.updateCache(tname,&(values[name]));
			return &(values[name]);
		}
	}else{
		sValue * temp2 = (sValue*)sc;
		return temp2;
	}
}

void qTable::setValuef(sValue & data, const char * fmt, ...){
	char text[1024];
	memset(text,0,1024);
	
	va_list ap;
	if (fmt==NULL) return;
	va_start(ap,fmt);
	vsprintf(text,fmt,ap);
	va_end(ap);
	setValue(text,data);
}

void qTable::setValueS(const char * name, sValue data){
	values[name] = data;
}

void qTable::setValue(const char * name, sValue data, const char * tname){
	void * sc=NULL;
	if (!tname){
		//do we have this key in cache?
		sc = tcache.searchCache(name);
		tname = name;
	}

	if (!sc){
		char * _name = _strdup(name);
		const char deliminator[]="::";
		char * result;
		map<string,sValue>::iterator temp;

		result = strstr(_name,deliminator);
		//there is a deliminator in path
		if (result){
			//remove a deliminator from the begining of the result
			result+=2;
			strtok(_name,deliminator);
		
			//let's check if this table exists
			temp = values.find(_name);
			//if not, let's createa new one
			if( temp == values.end() ) {
				values[_name].table = new qTable();
			}
			//proceed...
			values[_name].table->setValue(result,data,tname);
			free(_name);
		}else{
			//update key cache
			tcache.updateCache(tname,&(values[name]));
			//no further tables are needed so we can write a value here
			values[name] = data;
		}
	}else{
		//use pointer from cache
		sValue * temp2 = (sValue*)sc;
		(*temp2) = data;
	}
}

const char * qTable::getParent(const char * name){
	const char * temp = strrchr(name,':');
	ulong newlength = strlen(name)-strlen(temp);
	//we use _temp value in class so we don't have to take care of it
	strncpy(_temp,name,newlength-1);
	*(_temp+newlength-1) = '\0';
	return _temp;
}

const char * qTable::getSelf(const char * name){
	const char * temp = strrchr(name,':');
	ulong newlength = strlen(name)-strlen(temp);
	//we use _temp value in class so we don't have to take care of it
	strncpy(_temp,name,newlength+1);
	strcpy(_temp+newlength+1,"self");
	return _temp;
}
