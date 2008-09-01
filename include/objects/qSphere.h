#ifndef H_QSPHERE
#define H_QSPHERE

#include "qCircle.h"

class qSphere : public qCircle{
protected:
	qVector angle;
	qVector front,side;
public:
	qSphere();
	qSphere(qVector pos, qVector angle, float radius);
	~qSphere();
	void setAngle(qVector angle);
	qVector getAngle();
	void update(qVector * intersection=0,qVector * intersection2=0,qVector * normal=0);

	qVector getFront();
	qVector getSide();
};

#endif