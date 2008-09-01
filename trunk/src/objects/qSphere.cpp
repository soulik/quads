#include "objects/qSphere.h"
#include <math.h>
#include <float.h>
#include <stdio.h>

qSphere::qSphere(){
}

qSphere::qSphere(qVector pos, qVector angle, float radius) : qCircle(pos, radius){
	setAngle(angle);
}

qSphere::~qSphere(){
}

void qSphere::setAngle(qVector angle){
	this->angle = angle;

	if (this->angle.x>=360) this->angle.x -= 360;
	if (this->angle.y>=360) this->angle.y -= 360;
	if (this->angle.z>=360) this->angle.z -= 360;

	if (this->angle.x<0) this->angle.x += 360;
	if (this->angle.y<0) this->angle.y += 360;
	if (this->angle.z<0) this->angle.z += 360;
}

qVector qSphere::getAngle(){
	return angle;
}

void qSphere::update(qVector * intersection,qVector * intersection2, qVector * normal){
	front.x = -(float)sin(angle.z/180*M_PI)*(float)sin(angle.x/180*M_PI);
	front.y = (float)cos(angle.z/180*M_PI)*(float)sin(angle.x/180*M_PI);
	front.z = (float)cos(angle.x/180*M_PI);

	//left side
	side.x = (float)cos(angle.z/180*M_PI)*(float)sin(angle.x/180*M_PI);
	side.y = (float)sin(angle.z/180*M_PI)*(float)sin(angle.x/180*M_PI);
	side.z = 0;

	qObject::update(intersection);
	if (intersection&&normal){
		qVector pp,ppp;
		qVector speed2 = speed+((*normal).normalize()*speed.length());
		qVector L =	position-(*intersection);
		float t1,t2,d2,something,r2;
		t1 = (L&(speed.normalize()));
		
		{//if (t1>=0){
			d2 = (L&L)-(t1*t1);
			r2 = getRadius()*getRadius();
			/* Ahh, stupid floating point bug */
			if (r2>d2){
				t2 = sqrt(r2 - d2);
			}else{
				t2 = 0;
			}
			something = t1+t2;
			ppp=(*intersection)+speed.normalize()*something;
			pp=(position-ppp);
			//position = position + speed.normalize()*something;// + speed2;//(position-(*intersection))*getRadius()//(*intersection)+(*normal)*getRadius() + speed;
			
		}
		position = (*intersection) + pp +speed2;
	}

}

qVector qSphere::getFront(){
	return front;
}

qVector qSphere::getSide(){
	return side;
}