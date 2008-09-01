#ifndef GAME_H_
#define GAME_H_

#include "controlers/keyboard.h"
#include "controlers/mouse.h"
//#include <SDL/sdl_net.h>
#include "module.h"
#include "modules/script.h"
#include "game/gamelogic.h"

class cGame: public cModule{
private:
	SDL_Event event;
public:
	cGame(cBase * parent);
	virtual ~cGame();
	
	void init();
	void run();
	void stop();
	void requestStop();
	void postRun();
	void processEvents();
	cGamelogic * glogic;
};

#endif /*GAME_H_*/
