#include "utils/qRGBA.h"

qRGBA::qRGBA(){
	r = 0;
	g = 0;
	b = 0;
	a = 0;
}

qRGBA::qRGBA(float r, float g, float b){
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = 1;
}

qRGBA::qRGBA(float r, float g, float b, float a){
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}

qRGBA::qRGBA(unsigned int rgba){
	setRGBA(rgba);
}

void qRGBA::setRGBA(unsigned int rgba){
	r = (float)((rgba & 0xFF000000) >> 0x18)/255.0f;
	g = (float)((rgba & 0xFF0000) >> 0x10)/255.0f;
	b = (float)((rgba & 0xFF00) >> 0x8)/255.0f;
	a = (float)((rgba & 0xFF) )/255.0f;
}