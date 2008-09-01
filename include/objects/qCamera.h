#ifndef H_QCAMERA
#define H_QCAMERA

#include "qSphere.h"
#include "qPoint.h"

class qCamera : public qSphere {
protected:
	float aspectRatio;
	float fov;
public:
	qCamera();
	~qCamera();

	void setup();
	void setup2();
	void setAngleFromRelMouse(qPointi m_pos);
	void lookAt(qVector pos, qVector angle, float distance);
};

#endif