#ifndef QCIRCLE_H_
#define QCIRCLE_H_

#include "qObject.h"

class qCircle : public qObject{
private:
	float radius;
public:
	qCircle();
	qCircle(qVector pos, float radius);
	void setRadius(float radius);
	float getRadius();
	virtual ~qCircle();
	void update();
};

#endif /*QCIRCLE_H_*/
