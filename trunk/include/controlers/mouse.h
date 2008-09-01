#ifndef MOUSE_H_
#define MOUSE_H_

#include "objects/qPoint.h"
#include "qDebug.h"

class cMouse: public qDebug {
	qPointi position;
	qPointi _position;
	qPointi rel_pos;
	unsigned char buttons;
	unsigned char _buttons;
public:
	cMouse();

	void update();
	qPointi getPos();
	qPointi getPos2();
	qPointi getRelPos();
	unsigned char getButtons();
	unsigned char getPrevButtons();
	void setPos(int x, int y);
	void setRelPos(int x, int y);
	void synchronizePosition();
	void setButtons(unsigned int state);
	void setButton(unsigned int button, unsigned char state);
	
	virtual ~cMouse();
};

#endif /*MOUSE_H_*/
