#include "modules/controler.h"
#include "modules/game.h"
#include "modules/renderer.h"
#include "base.h"
#include "common.h"

cControler::cControler(cBase * parent): cModule(parent){
	setName("cControler");
	try{
		memset(keys,0,256*sizeof(int));

		keyboard = new cKeyboard(MAX_KEYS_NUM);
		if (!keyboard) throw cException("Can't create keyboard interface");

		mouse = new cMouse();
		if (!mouse) throw cException("Can't create mouse interface");
	}
	DEFAULT_EXCEPTION(stderr)
#ifdef DEBUGGING
	dprintf("cControler()\n");
#endif

}

cControler::~cControler(){
	delete mouse;
	delete keyboard;
}

void cControler::setKeys(int * k){
	memcpy(keys,k,256*sizeof(int));
}

void cControler::processKeys(){
	if (keyboard->GetKey(SDLK_TAB)){
		GAME->glogic->switchCamera();
	}
	RENDER->updateEvents();

	if (keyboard->GetMod()&KMOD_SHIFT){
		GAME->glogic->show_cursor = 0;
	}else{
		GAME->glogic->show_cursor = 1;
	}
}

void cControler::init(){
	status = TS_RUNNING;
}

void cControler::stop(){
	status = 0;
}