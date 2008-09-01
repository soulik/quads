#ifndef QPOINT_H_
#define QPOINT_H_

class qPoint{
public:
	float x,y;
	qPoint(float x, float y);
	qPoint();
	qPoint normalize();
	void set(float x, float y);
};

class qPointi{
public:
	int x,y;
	qPointi(int x, int y);
	qPointi();
	qPointi normalize();
	void set(int x, int y);
};

/*

//Damn, this has worked in GCC...

struct qPoint {
	float x,y;
} qPoint = {0,0};

struct qPointi {
	int x,y;
} qPointi = {0,0};
*/

#endif /*QPOINT_H_*/
