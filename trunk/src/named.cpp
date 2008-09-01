#include "named.h"
#include <malloc.h>
#include <string.h>

cNamed::cNamed(){
	name = (char*) malloc(32);
	setName("Unnamed");
}

cNamed::~cNamed(){
	if (name) free(name);
}

const char * cNamed::getName(){
	return name;
}

void cNamed::setName(const char *_name){
	if (name) free(name);
	name = _strdup(_name);
}