#include "gui/qDrawable.h"

qDrawable::qDrawable(){
	flags = 0;
	id = 0;
}

qDrawable::~qDrawable(){
}

int qDrawable::draw(){
	return 0;
}

unsigned int qDrawable::getID(){
	return id;
}

unsigned int qDrawable::getFlags(){
	return flags;
}

void qDrawable::setFlags(unsigned int flags){
	this->flags = flags;
}