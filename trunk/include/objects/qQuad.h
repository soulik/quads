#ifndef QQUAD_H_
#define QQUAD_H_

#include "qObject.h"

class qQuad : public qObject {
private:
	float width, height;
public:
	qQuad();
	qQuad(qVector pos, float width, float height);
	virtual ~qQuad();
	
	void setWidth(float width);
	float getWidth();
	
	void setHeight(float height);
	float getHeight();
};

#endif /*QQUAD_H_*/
