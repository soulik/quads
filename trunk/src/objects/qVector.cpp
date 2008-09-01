#include "objects/qVector.h"
#include <math.h>

qVector::qVector(){
	x = 0;
	y = 0;
	z = 0;
}

qVector::qVector(float x, float y, float z){
	this->x = x;
	this->y = y;
	this->z = z;
}

qVector qVector::operator+(const qVector &other){
	return qVector(x + other.x, y + other.y, z + other.z);
}

qVector qVector::operator-(const qVector &other){
	return qVector(x - other.x, y - other.y, z - other.z);
}

qVector qVector::operator*(const qVector &other){
	return qVector(x * other.x, y * other.y, z * other.z);
}

qVector qVector::operator/(const qVector &other){
	return qVector(x / other.x, y / other.y, z / other.z);
}

qVector qVector::operator|(const qVector &other){
	float xx = y*other.z - z*other.y;
	float yy = z*other.x - x*other.z;
	float zz = x*other.y - y*other.x;
	return qVector(xx,yy,zz);
}

float qVector::operator&(const qVector &other){
	return x*other.x + y*other.y + z*other.z;
}

qVector qVector::operator+(float other){
	return qVector(x + other, y + other, z + other);
}

qVector qVector::operator-(float other){
	return qVector(x - other, y - other, z - other);
}

qVector qVector::operator*(float other){
	return qVector(x * other, y * other, z * other);
}

qVector qVector::operator/(float other){
	return qVector(x / other, y / other, z / other);
}

qVector qVector::operator+(const qPoint &other){
	return qVector(x + other.x, y + other.y, z);
}

qVector qVector::operator-(const qPoint &other){
	return qVector(x - other.x, y - other.y, z);
}

qVector qVector::operator+(const qPointi &other){
	return qVector(x + (float)other.x, y + (float)other.y, z);
}

qVector qVector::operator-(const qPointi &other){
	return qVector(x - (float)other.x, y - (float)other.y, z);
}

void qVector::operator+=(const qVector &other){
	x += other.x;
	y += other.y;
	z += other.z;
}

void qVector::operator-=(const qVector &other){
	x -= other.x;
	y -= other.y;
	z -= other.z;
}

void qVector::operator*=(const qVector &other){
	x *= other.x;
	y *= other.y;
	z *= other.z;
}

void qVector::operator/=(const qVector &other){
	x /= other.x;
	y /= other.y;
	z /= other.z;
}

void qVector::operator+=(float other){
	x += other;
	y += other;
	z += other;
}

void qVector::operator-=(float other){
	x -= other;
	y -= other;
	z -= other;
}

void qVector::operator*=(float other){
	x *= other;
	y *= other;
	z *= other;
}

void qVector::operator/=(float other){
	x /= other;
	y /= other;
	z /= other;
}

void qVector::operator+=(const qPoint &other){
	x += other.x;
	y += other.y;
}

void qVector::operator-=(const qPoint &other){
	x -= other.x;
	y -= other.y;
}

void qVector::operator+=(const qPointi &other){
	x += (float)other.x;
	y += (float)other.y;
}

void qVector::operator-=(const qPointi &other){
	x -= (float)other.x;
	y -= (float)other.y;
}


qVector qVector::operator=(const qVector & other){
	x = other.x;
	y = other.y;
	z = other.z;
	return *this;
}

int qVector::operator==(const qVector & other){
	if ((x==other.x)&&(y==other.y)&&(z==other.z)) return 1; else return 0;
}

qVector qVector::operator=(float other){
	x = other;
	y = other;
	z = other;
	return *this;
}

int qVector::operator==(float other){
	if ((x==other)&&(y==other)&&(z==other)) return 1; else return 0;
}

qVector qVector::operator=(const qPoint & other){
	x = other.x;
	y = other.y;
	return *this;
}

qVector qVector::operator=(const qPointi & other){
	x = (float)other.x;
	y = (float)other.y;
	return *this;
}

float qVector::length(){
	return sqrtf(x*x + y*y + z*z);
}

qVector qVector::normalize(){
	float len = this->length();
	if (len==0.0f) return qVector(0,0,0);
	return qVector(x/len,y/len,z/len);
}

void qVector::set(float x, float y, float z){
	this->x = x;
	this->y = y;
	this->z = z;
}