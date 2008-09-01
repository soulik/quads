#ifndef H_GUI
#define H_GUI

#include "objects/qVector.h"
#include "utils/qRGBA.h"
#include "module.h"
#include "gui/qDrawable.h"
#include <vector>

class cGui: public cModule{
private:
	qDrawable* desktop_events;
	vector<SDL_Cursor*> cursors;

public:
	cGui(cBase * parent);
	~cGui();

	vector<qDrawable*> dObjects;

	void drawBox(int x, int y, int width, int height);
	void _drawBox(int x, int y, int width, int height, qRGBA color = qRGBA(1,1,1,1));
	void drawBox2(int x, int y, int width, int height);
	void drawPoint(qVector pos, qRGBA color, float size = 16.0f);
	void drawCircle(int x, int y, int radius, int seg, float alpha);
	void drawTriangle(qVector a, qVector b, qVector c);

	void initWindows();
	void updateEvents();
	void updateWindows();
	void drawWindows();

	int addCursor(unsigned char * data,unsigned char * mask, int width, int height, int hotx, int hoty);
	SDL_Cursor * getCursor(int id);
};

#endif