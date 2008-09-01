#ifndef H_QVECTOR
#define H_QVECTOR

#include "objects/qPoint.h"

class qVector {
public:
	float x,y,z;
	qVector();
	qVector(float x, float y, float z);

	void set(float x, float y, float z);

	qVector operator+(const qVector & other);
	qVector operator-(const qVector & other);
	qVector operator*(const qVector & other);
	qVector operator/(const qVector & other);
	qVector operator|(const qVector & other);
	float operator&(const qVector & other);

	qVector operator+(float other);
	qVector operator-(float other);
	qVector operator*(float other);
	qVector operator/(float other);

	qVector operator+(const qPoint & other);
	qVector operator-(const qPoint & other);
	qVector operator+(const qPointi & other);
	qVector operator-(const qPointi & other);

	void operator+=(const qVector & other);
	void operator-=(const qVector & other);
	void operator*=(const qVector & other);
	void operator/=(const qVector & other);

	void operator+=(float other);
	void operator-=(float other);
	void operator*=(float other);
	void operator/=(float other);

	void operator+=(const qPoint & other);
	void operator-=(const qPoint & other);
	void operator+=(const qPointi & other);
	void operator-=(const qPointi & other);

	qVector operator=(const qVector & other);
	int operator==(const qVector & other);

	qVector operator=(float other);
	int operator==(float other);

	qVector operator=(const qPoint & other);
	qVector operator=(const qPointi & other);

	qVector normalize();
	float length();
};

#endif