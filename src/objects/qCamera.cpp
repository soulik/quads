#include "objects/qCamera.h"
#include <math.h>
#include <sdl/SDL.h>
#include <SDL/SDL_opengl.h>

qCamera::qCamera() {
	aspectRatio = 4/3;
	fov = 45.0f;
	friction = 1.5f;
	setRadius(4);
	setPosition(qVector(0,0,-15));
	setAngle(qVector(90,0,0));
}

qCamera::~qCamera(){
}

void qCamera::setAngleFromRelMouse(qPointi m_pos){
	qVector ang = getAngle();

	ang.x += m_pos.y;
	ang.y = 0;
	ang.z += m_pos.x;

	setAngle(ang);
}

void qCamera::lookAt(qVector pos, qVector angle, float distance){
	qVector _pos = qVector(	pos.x-sin((angle.z)/180*M_PI)*sin(angle.x/180*M_PI)*distance,
							pos.y+cos((angle.z)/180*M_PI)*sin(angle.x/180*M_PI)*distance,
							pos.z+cos(angle.x/180*M_PI)*distance
						);
	qVector _angle = qVector(angle.x,0,angle.z);
	/*
	front.x = -(float)sin(angle.z/180*M_PI)*(float)sin(angle.x/180*M_PI);
	front.y = (float)cos(angle.z/180*M_PI)*(float)sin(angle.x/180*M_PI);
	front.z = (float)cos(angle.x/180*M_PI);
*/
	setPosition(_pos);
	setAngle(_angle);
}

void qCamera::setup(){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov,aspectRatio,0.1f,10000.0f);
		
	glRotatef(angle.x,1,0,0);
	glRotatef(angle.z,0,0,1);

	glTranslatef(-position.x,-position.y,-position.z);

	//glMatrixMode(GL_MODELVIEW);

}

void qCamera::setup2(){
	gluPerspective(fov,aspectRatio,0.1f,10000.0f);
	glRotatef(angle.x,1,0,0);
	glRotatef(angle.z,0,0,-1);
	glTranslatef(-position.x,-position.y,-position.z);
}