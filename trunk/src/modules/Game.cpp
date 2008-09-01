#include "modules/game.h"
#include "modules/controler.h"
#include "game/gamelogic.h"
#include "errors/exception.h"
#include "main.h"

#define MAIN ((Main*)this->parent)

cGame::cGame(cBase * parent): cModule(parent){
	threaded=1;
	setName("cGame");
#ifdef DEBUGGING
	dprintf("cGame()\n");
#endif
}

cGame::~cGame(){
	if (glogic){
		delete glogic;
	}
}

void cGame::init(){
	status = TS_RUNNING;
	try{
		glogic = new cGamelogic(parent);
		if (!glogic) throw cException("Can't create game logic");
//		SCRIPT->call("init_game");
//		SCRIPT->call("initRenderer");
/*
		if (SDLNet_Init()==-1) throw Exception("Can't init sdl_net");

		udpsock=SDLNet_UDP_Open(6666);
		if(!udpsock) {
			printf("SDLNet_UDP_Open: %s\n", SDLNet_GetError());
			throw Exception("Can't open UDP");
		}
		int channel;
		IPaddress address;
		address.host = 0x0100007F;
		address.port = 0x0A1A0000;
		channel=SDLNet_UDP_Bind(udpsock, -1, &address);
		if(channel==-1) {
			printf("SDLNet_UDP_Bind: %s\n", SDLNet_GetError());
			throw Exception("Can't bind");
		}
		*/
		this->setRunnable(this);
		dprintf("Looks good.\n");
		//start();
	}
	DEFAULT_EXCEPTION(stderr)
}

void cGame::processEvents(){
	glogic->processCursor();
	if (SDL_PollEvent( &event )){
		// process events
		switch( event.type ){
		case SDL_MOUSEBUTTONUP:
		case SDL_MOUSEBUTTONDOWN:
			CONTROL->getMouse()->setButton(event.button.button,event.button.state);
			break;
		case SDL_MOUSEMOTION:
			CONTROL->getMouse()->setPos(event.motion.x,event.motion.y);
			CONTROL->getMouse()->setRelPos(event.motion.xrel,event.motion.yrel);
			CONTROL->getMouse()->setButtons(event.motion.state);
			CONTROL->getMouse()->synchronizePosition();
			break;
		//case SDL_
		case SDL_KEYDOWN:
			CONTROL->getKeyboard()->SetKey(event.key.keysym.sym,1);
			break;
		case SDL_KEYUP:
			CONTROL->getKeyboard()->SetKey(event.key.keysym.sym,0);
			break;
		case SDL_QUIT:
			dprintf("Quit requested\n");
			status &= ~TS_RUNNING;
			break;
		}
	}
}

void cGame::run(){
	while (status&TS_RUNNING){
		CONTROL->getMouse()->update();
		if (glogic){
			glogic->resetRelativePosition();
			CONTROL->processKeys();
			glogic->ProcessGame();
			glogic->applyRelativePosition();
		}
	}
	dprintf("Quiting...\n");
	parent->setStatus(0);
	return;
}

void cGame::postRun(){
	SCRIPT->call("init_game");
	SCRIPT->call("init_renderer");
}

void cGame::requestStop(){
	status &= ~TS_RUNNING;
	parent->setStatus(0);
}

void cGame::stop(){
	/*
	SDLNet_UDP_Unbind(udpsock, 0);	
	SDLNet_UDP_Close(udpsock);
	udpsock=NULL;
	SDLNet_Quit();
	*/
	requestStop();
}
