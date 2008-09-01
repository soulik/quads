#include "objects/qCircle.h"

qCircle::qCircle(){
}

qCircle::qCircle(qVector pos, float radius) : qObject(pos){
	setRadius(radius);
}
/*
qCircle::qCircle(float x ,float y, float radius) : qObject(qPoint(x,y)){
	setRadius(radius);
}
  */
qCircle::~qCircle(){
}

void qCircle::setRadius(float radius){
	this->radius = radius;
}

float qCircle::getRadius(){
	return radius;
}

void qCircle::update(){
	qObject::update();
	/*
		Can't go through walls
	*/
	if (position.x<0) position.x = 0;
	if (position.y<0) position.y = 0;
	if (position.x>320) position.x = 320;
	if (position.y>200) position.y = 200;
	
	/*
		Bouncing
	*/
	
	if (((position.x+speed.x)<0)||((position.x+speed.x)>320)) speed.x *= -elasticity;
	if (((position.y+speed.y)<0)||((position.y+speed.y)>200)) speed.y *= -elasticity;

}
