#include "objects/qObject.h"
#include <stdio.h>
#include <float.h>

qObject::qObject(){
	setState(	qVector(0,0,0),
				qVector(0,0,0),
				qVector(0,0,0));
	setFriction(1.0f);
	setElasticity(1.0f);
}

qObject::qObject(qVector pos) {
	setState(pos,qVector(0,0,0),qVector(0,0,0));
	setFriction(1.0f);
	setElasticity(1.0f);
}

qObject::qObject(qVector pos, qVector speed){
	//setState(pos,speed,qPoint(0,0));
	setFriction(1.0f);
	setElasticity(1.0f);
}

qObject::qObject(qVector pos, qVector speed, qVector accel){
	setState(pos,speed,accel);
	setFriction(1.0f);
	setElasticity(1.0f);
}

void qObject::setState(qVector pos, qVector speed, qVector accel){
	setPosition(pos);
	setSpeed(speed);
	setAcceleration(accel);
	/*
	setFriction(1.0f);
	setElasticity(1.0f);
	*/
}

void qObject::setPosition(qVector pos){
	this->position = pos;
}

void qObject::setSpeed(qVector speed){
	this->speed = speed;
}

void qObject::setAcceleration(qVector acceleration){
	this->acceleration = acceleration;
}

void qObject::setElasticity(float elasticity){
	this->elasticity = elasticity;
}

void qObject::setFriction(float friction){
	this->friction = friction;
}

qObject::~qObject(){
}

void qObject::update(qVector * intersection){
	/*
		hmm gravitation?..
	*/
	//acceleration.y += 0.99f;

	/*
		friction
	*/
	if (friction != 0.0f) speed /= friction;

	/*
		speed
	*/

	speed += acceleration;
	if (_isnan(speed.x)){
		printf("Ops!!! Speed! \n");
	}

	
	/*
	acceleration = qVector(0,0,0);
	*/
	if (!intersection){
		position += speed;
	}
	
}
