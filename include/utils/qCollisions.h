#ifndef H_QCOLLISIONS
#define H_QCOLLISIONS

#include "objects/qVector.h"
#include "objects/qSphere.h"

class cGamelogic;

class qCollisions {
	qSphere * sphere;
	qVector * triangle[3];
public:
	qCollisions();
	~qCollisions();

	int last_result;
	qVector intersection, intersection2;
	qVector ints[6];
	float distance, test1, test2, test3,test4;
	int colTest;
	qVector nnn;
	
	qVector closestPointOnLine(qVector a, qVector b, qVector p);
	qVector closestPointOnTriangle(qVector *triangle, qVector p);
	int isInsideTriangle(qVector * triangle, qVector p);
	int sameSide (qVector p1, qVector p2,qVector a, qVector b);
	int test(qSphere * sphere, qVector * triangle);
	int test2ex(qSphere * sphere, qVector * triangle);
	int find(qSphere * sphere, qVector * triangle, qVector * out);
	void planeNormal(qVector * triangle, qVector * normal);
	float getHeightFromHMap(qVector _pos, cGamelogic * glogic);
	
};

#endif