#include "objects/qQuad.h"

qQuad::qQuad(){
	setPosition(qVector(0,0,0));
	setWidth(0);
	setHeight(0);
}

qQuad::qQuad(qVector pos, float width, float height){
	setPosition(pos);
	setWidth(width);
	setHeight(height);
}

qQuad::~qQuad(){
}

float qQuad::getWidth(){
	return width;
}

float qQuad::getHeight(){
	return height;
}

void qQuad::setWidth(float width){
	this->width = width;
}

void qQuad::setHeight(float height){
	this->height = height;
}
