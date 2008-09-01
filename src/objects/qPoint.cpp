#include "objects/qPoint.h"
#include <math.h>

/*
	with float 
*/

qPoint::qPoint(float x, float y){
	this->x = x;
	this->y = y;
}

qPoint::qPoint(){
	qPoint(0,0);
}

qPoint qPoint::normalize(){
	float divisor = sqrtf(x*x+y*y);
	float x = this->x, y = this->y;
	if (divisor!=0.0f){
		x /= divisor;
		y /= divisor;
	}
	return qPoint(x,y);
}

void qPoint::set(float x, float y){
	this->x = x;
	this->y = y;
}

/*
	with integer 
*/

qPointi::qPointi(int x, int y){
	this->x = x;
	this->y = y;
}

qPointi::qPointi(){
	qPointi(0,0);
}

qPointi qPointi::normalize(){
	int divisor = (int) sqrt(float(x*x) + float(y*y));
	int x = this->x, y = this->y;
	if (divisor!=0){
		x /= divisor;
		y /= divisor;
	}
	return qPointi(x,y);
}

void qPointi::set(int x, int y){
	this->x = x;
	this->y = y;
}
