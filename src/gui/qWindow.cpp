#include "gui/qWindow.h"
#include <SDL/SDL_opengl.h>
#include <stdio.h>
#include "utils/qUtils.h"
#include "base.h"
#include "common.h"
#include "modules/script.h"

qWindow::qWindow(){
	this->x = 0;
	this->y = 0;
	this->width = 0;
	this->height = 0;
	this->caption = NULL;
	this->context = NULL;
	id = 0;
	setName("qWindow");
	this->type = 1;
	this->func.self = 0;
	this->func.setup = 0;
	this->func.update = 0;
}

qWindow::qWindow(float x, float y, float width, float height){
	caption = NULL;
	context = NULL;
	setParam(x,y,width,height);
	id = 0;
	setName("qWindow");
	this->type = 1;
	this->func.self = 0;
	this->func.setup = 0;
	this->func.update = 0;
}

qWindow::~qWindow(){
}

void qWindow::setParam(float x, float y, float width, float height){
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
}

void qWindow::setCaption(const char * string){
	caption = qUtils::safe_strcpy(caption,string);
	//caption = string;
}

void qWindow::setContext(const char * string){
	//context = string;
	context = qUtils::safe_strcpy(context,string);
}

void qWindow::sendEvent(int event_num, int param1, int param2){
	int ret = __SCRIPT->callLF(func.event, func.self, event_num, param1, param2);
	dprintf("Event returned %d\n",ret);
}

void qWindow::drawContext(){
	//cRenderer * rend = getRenderer();
	if (context)
		RENDER->font->printf(x+2,y+2+24+4,qRGBA(0xFFFFC0B0),context);
}

int qWindow::draw(){

	if (flags & QF_VISIBLE){
		RENDER->prepareOrthogonal();

		//cRenderer * rend = getRenderer();
		qRGBA color = qRGBA(0xFFFF00FF);

		glBindTexture(GL_TEXTURE_2D, 3);
		glEnable(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

/*	Window body */
		glBegin(GL_QUADS);
			glColor4f(1,1,1,0.7f);
			glTexCoord2f(0,0);
			glVertex2f(x,y);
			glTexCoord2f(1,0);
			glVertex2f(x+width,y);
			glColor4f(1,1,1,1.0f);
			glTexCoord2f(1,1);
			glVertex2f(x+width,y+height);
			glTexCoord2f(0,1);
			glVertex2f(x,y+height);
		glEnd();

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glDisable(GL_TEXTURE_2D);

/*	Title box */
		glBegin(GL_QUADS);
			glColor4f(0,0,0.5f,0.7f);
			glVertex2f(x+2,y+2);
			glColor4f(0,0,0.0f,0.7f);
			glVertex2f(x+width-2,y+2);
			glVertex2f(x+width-2,y+24+2);
			glColor4f(0,0,0.5f,0.7f);
			glVertex2f(x+2,y+24+2);
		glEnd();
		
/*	Border line */
		glBegin(GL_LINE_STRIP);
			glColor4f(0.9f,0.9f,0.9f,0.7f);
			glVertex2f(x,y);
			glColor4f(0.5f,0.5f,0.5f,0.7f);
			glVertex2f(x+width,y);
			glColor4f(0.1f,0.1f,0.1f,0.7f);
			glVertex2f(x+width,y+height);
			glColor4f(0.5f,0.5f,0.5f,0.7f);
			glVertex2f(x,y+height);
			glColor4f(0.9f,0.9f,0.9f,0.7f);
			glVertex2f(x,y);
		glEnd();

		glBlendFunc(GL_SRC_ALPHA,GL_ONE);
		glDisable(GL_BLEND);

		if (caption)
			RENDER->font->printf(x+2,y+2+4,qRGBA(0xFFFFFFFF),caption);
		drawContext();
	}
	return 1;
}