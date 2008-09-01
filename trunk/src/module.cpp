#include "module.h"

cModule::~cModule(){
#ifndef NO_MUTEX
	SDL_mutexP(mutex);
	SDL_mutexV(mutex);

	SDL_DestroyMutex(mutex);
#endif
}

void cModule::lockMutex(){
	SDL_mutexP(mutex);
}

void cModule::unlockMutex(){
	SDL_mutexV(mutex);
}

void cModule::sendMessage(char *msg){
	if (!msg) return;
	if (!strcmp(msg,"stop")){
		this->stop();
	}
}