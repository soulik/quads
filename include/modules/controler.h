#ifndef H_CONTROLER
#define H_CONTROLER

#include "controlers/keyboard.h"
#include "controlers/mouse.h"
#include "module.h"

enum cKeys {
	K_FORWARD,
	K_BACKWARD,
	K_STRAFE_LEFT,
	K_STRAFE_RIGHT,
	K_UP,
	K_DOWN,
	K_QUIT
};

class cControler: public cModule {
private:
	cKeyboard * keyboard;
	cMouse * mouse;
	int keys[256];
	
public:
	cControler(cBase * parent);
	~cControler();

	inline cKeyboard * getKeyboard(){
		return keyboard;
	}
	inline cMouse * getMouse(){
		return mouse;
	}
	void setKeys(int * keys);
	void processKeys();
	void init();
	void stop();
};

#endif