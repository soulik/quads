#ifndef QOBJECT_H_
#define QOBJECT_H_

#include "qVector.h"

class qObject{
private:
	void setState(qVector pos, qVector speed, qVector accel);
protected:
	qVector position, speed, acceleration;
	float elasticity, friction;
public:
	qObject();
	qObject(qVector pos);
	qObject(qVector pos, qVector speed);
	qObject(qVector pos, qVector speed, qVector accel);

	inline qVector getPosition(){
		return position;
	}
	void setPosition(qVector pos);

	inline qVector getSpeed(){
		return speed;
	}
	void setSpeed(qVector speed);

	inline qVector getAcceleration(){
		return acceleration;
	}
	void setAcceleration(qVector acceleration);
	
	inline float getElasticity(){
		return elasticity;
	}
	void setElasticity(float elasticity);
	
	inline float getFriction(){
		return friction;
	}
	void setFriction(float friction);

	void update(qVector * intersection=0);

	virtual ~qObject();
};

#endif /*QOBJECT_H_*/
