#ifndef H_QWINDOW
#define H_QWINDOW

#include "gui/qDrawable.h"
#include "modules/renderer.h"
#include "qDebug.h"

/*
	FLAGS
	--------+------------
	bit		| explanation
	--------+------------	
	0		- VISIBLE
	1		- ENABLED
	2-3		- SIZE
	4-31	- RESERVED

*/

#define QF_ENABLED		0x00000002

#define QF_MINIMIZED	0x00000004
#define QF_SHADED		0x00000005
#define QF_NORMAL		0x00000006
#define QF_MAXIMIZED	0x00000007

struct qWindowFunctions{
	int setup;
	int update;
	int event;
	int self;
};

class qPlane {
protected:
	float x,y;
	float width,height;
public:
	qPlane(){
		x = 0;
		y = 0;
		width = 0;
		height = 0;
	}
	int isIn(float _x, float _y){
		if ((_x<x) || (_y<y) || (_x>x+width) || (_y>y+height))
			return 0;
		else
			return 1;
	}
};


class qWindow : public qDrawable, public qDebug, public qPlane{
protected:
	char * caption;
	char * context;
	void drawContext();

public:
	qWindow();
	qWindow(float x, float y, float width, float height);
	~qWindow();

	void setID(unsigned int id){
		this->id = id;
		dprintf("Window got new ID: %d\n",id);
	}
	
	int draw();
	void setParam(float x, float y, float width, float height);
	void setCaption(const char * string);
	void setContext(const char * string);
	void sendEvent(int event_num, int param1, int param2);
	qWindowFunctions func;
};

#endif