#include "gui/gui.h"
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <math.h>
#include "modules/game.h"
#include "modules/renderer.h"
#include "modules/qTexturer.h"
#include "modules/controler.h"
#include "base.h"
#include "common.h"
#include "gui/qWindow.h"

extern cBase * __parent;

class qDesktop :public qWindow{
private:
	SDL_mutex * mutex;
public:
	qDesktop();
	qDesktop(float x, float y, float width,float height);
	~qDesktop();
	int draw();
};

qDesktop::qDesktop() : qWindow(0, 0, 0, 0){
#ifndef NO_MUTEX
	mutex = SDL_CreateMutex();
#endif
}

qDesktop::qDesktop(float x, float y, float width,float height) : qWindow(x, y, width, height){
#ifndef NO_MUTEX
	mutex = SDL_CreateMutex();
#endif
}

qDesktop::~qDesktop(){
#ifndef NO_MUTEX
	SDL_DestroyMutex(mutex);
#endif
}

int qDesktop::draw(){
#ifndef NO_MUTEX
	SDL_mutexP(mutex);
#endif
	__RENDER->drawWindows();
#ifndef NO_MUTEX
	SDL_mutexV(mutex);
#endif
	return 1;
}

cGui::cGui(cBase * parent) : cModule(parent){
	desktop_events = NULL;
}

cGui::~cGui(){
	for (unsigned int i=0; i<dObjects.size() ; i++){
		delete dObjects[i];
	}
	dObjects.clear();
	for (unsigned int i=0; i<cursors.size() ; i++){
		SDL_FreeCursor(cursors[i]);
	}
	cursors.clear();
}

int cGui::addCursor(unsigned char * data,unsigned char * mask, int width, int height, int hotx, int hoty){
	SDL_Cursor * temp = SDL_CreateCursor(data,mask,width,height,hotx,hoty);
	cursors.push_back(temp);
	return cursors.size()-1;
}

SDL_Cursor * cGui::getCursor(int id){
	SDL_Cursor * temp = NULL;
	if (id>=0 && id < cursors.size()){
		temp = this->cursors[id];
		return temp;
	}
	return NULL;
}

void cGui::drawBox(int x, int y, int width, int height){
	glBindTexture(GL_TEXTURE_2D,TEXTURE->getImage("textures\\base_floor\\clang_floor.jpg")->getID());
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
		
		glColor3f(1,1,1);
		glTexCoord2f(0,0);
		glVertex2i(x,y);

		glColor3f(1,1,1);
		glTexCoord2f(1,0);
		glVertex2i(width+x,y);

		glColor3f(1,1,1);
		glTexCoord2f(1,1);
		glVertex2i(width+x,height+y);

		glColor3f(1,1,1);
		glTexCoord2f(0,1);
		glVertex2i(x,height+y);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	qRGBA color= qRGBA(1,0,0,1);

	drawPoint(qVector((float)x,(float)y,0),color,4);
	drawPoint(qVector((float)width+x,(float)y,0),color,4);
	drawPoint(qVector((float)width+x,(float)height+y,0),color,4);
	drawPoint(qVector((float)x,(float)height+y,0),color,4);
/*
	glEnable(GL_BLEND);
	glBegin(GL_POINTS);
		glColor3f(1,0,0);
		glVertex2i(x,y);
		glColor3f(1,0,0);
		glVertex2i(width+x,y);
		glColor3f(1,0,0);
		glVertex2i(width+x,height+y);
		glColor3f(1,0,0);
		glVertex2i(x,height+y);
	glEnd();
	glDisable(GL_BLEND);
	*/
}

void cGui::_drawBox(int x, int y, int width, int height, qRGBA color){
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
		
		glColor4f(color.r,color.g,color.b,color.a);
		glTexCoord2f(0,0);
		glVertex2i(x,y);

		glColor4f(color.r,color.g,color.b,color.a);
		glTexCoord2f(1,0);
		glVertex2i(width+x,y);

		glColor4f(color.r,color.g,color.b,color.a);
		glTexCoord2f(1,1);
		glVertex2i(width+x,height+y);

		glColor4f(color.r,color.g,color.b,color.a);
		glTexCoord2f(0,1);
		glVertex2i(x,height+y);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void cGui::drawBox2(int x, int y, int width, int height){
	glEnable(GL_BLEND);
	glBegin(GL_QUADS);
		glColor4f(0,0,1,0.5f);
		glVertex2i(x,y);
		glColor4f(0.5f,0,1,0.5f);
		glVertex2i(width+x,y);
		glColor4f(1,0,1,0.5f);
		glVertex2i(width+x,height+y);
		glColor4f(0.5f,0,1,0.5f);
		glVertex2i(x,height+y);
	glEnd();
	glDisable(GL_BLEND);
}

void cGui::drawCircle(int x, int y, int radius, int seg, float alpha=0.5f){
	if (seg<3) return;
	glEnable(GL_BLEND);
	glBegin(GL_POLYGON);
		for (int i=0; i<360; i+=(360/seg)){
			glColor4f(0,1,i/360.0f,alpha);
			glVertex2i(x+int(cos(i*M_PI/180)*radius),y+int(sin(i*M_PI/180)*radius));
		}
	glEnd();

	glBegin(GL_POINTS);
		for (int i=0; i<360; i+=(360/seg)){
			glColor4f(1,0,0,alpha*1.5f);
			glVertex2i(x+int(cos(i*M_PI/180)*radius),y+int(sin(i*M_PI/180)*radius));
		}
		glColor4f(1,0,0,alpha*1.5f);
		glVertex2i(x,y);

	glEnd();
	glDisable(GL_BLEND);
}

void cGui::drawTriangle(qVector a, qVector b, qVector c){
	glBlendFunc(GL_SRC_ALPHA,GL_DST_ALPHA);
	glEnable(GL_BLEND);

	//qVector triangle[3];
	/*
	triangle[0].set(64,64,-10);
	triangle[1].set(80,80,-26);
	triangle[2].set(80,64,-10);
	  */
	glBegin(GL_TRIANGLES);
	(GAME->glogic->collisions.last_result>0) ? glColor4f(0,1,0,0.5f) : 	glColor4f(1,0,0,0.5f);

		glVertex3f(a.x,a.y,a.z);
		glVertex3f(c.x,c.y,c.z);
		glVertex3f(b.x,b.y,b.z);

		glVertex3f(a.x,a.y,a.z);
		glVertex3f(b.x,b.y,b.z);
		glVertex3f(c.x,c.y,c.z);

	glEnd();

	glDisable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);

	qRGBA color = (GAME->glogic->collisions.last_result>0) ? qRGBA(0,1,0,1) : qRGBA(1,0,0,1);

	drawPoint(a,color,8);
	drawPoint(b,color,8);
	drawPoint(c,color,8);

	/* normal */

	qVector _a = a - b;
	qVector _b = a - c;
	qVector n = _a|_b;
	n = n.normalize()*5;
	qVector mid = (a+b+c)/3;

	glBegin(GL_LINES);
		glColor4f(1,1,1,1);
		glVertex3f(mid.x,mid.y,mid.z);
		glVertex3f(mid.x+n.x,mid.y+n.y,mid.z+n.z);
	glEnd();

	glPointSize(1.0f);
}

void cGui::drawPoint(qVector pos, qRGBA color, float size){
	qVector angle = GAME->glogic->camera->getAngle(); //(GAME->glogic->camera->getPosition()-pos).normalize();

	__SCRIPT->call("drawPoint",pos.x,pos.y,pos.z,16,1,1,0,1);
	
}

void cGui::initWindows(){
	while (!(SCRIPT->getStatus() & TS_RUNNING2)){
		SDL_Delay(1);
	}
	qDrawable * w=new qDesktop();
	RENDER->dObjects.push_back(w);
}

void cGui::updateWindows(){
	while (!(SCRIPT->getStatus() & TS_RUNNING2)){
		SDL_Delay(1);
	}
	SCRIPT->lockMutex();
	SCRIPT->call("updateWindows");
	SCRIPT->unlockMutex();
}

void cGui::updateEvents(){
	while (!(SCRIPT->getStatus() & TS_RUNNING2)){
		SDL_Delay(1);
	}
	SCRIPT->lockMutex();
	SCRIPT->call("processWindowEvents");
	SCRIPT->unlockMutex();
}

void cGui::drawWindows(){
	while (!(SCRIPT->getStatus() & TS_RUNNING2)){
		SDL_Delay(1);
	}
	SCRIPT->lockMutex();
	SCRIPT->call("drawWindows");
	SCRIPT->unlockMutex();
}