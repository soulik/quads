#include "base.h"
#include "errors/exception.h"
#include "errors/errors.h"
#include <SDL/SDL.h>

cBase::cBase(){
	status=0;
}

cBase::~cBase(){
}

void cBase::initModules(){
	try{
		for (uint i=0; i<modules.size();i++){
			modules[i]->init();
		}
	}DEFAULT_EXCEPTION(this->stddbg)
}

void cBase::runModules(){
	try{
		status=1;
		for (uint i=0; i<modules.size();i++){
			modules[i]->preRun();
		}
		for (uint i=0; i<modules.size();i++){
			if (!modules[i]->hasThread()) modules[i]->run();
		}
		for (uint i=0; i<modules.size();i++){
			modules[i]->postRun();
		}
		for (uint i=0; i<modules.size();i++){
			//modules should be at least running before quiting
			while (!(modules[i]->getStatus()&TS_RUNNING)){
				SDL_Delay(1);
			}
		}
		while(status){
			SDL_Delay(1);
		}
	}DEFAULT_EXCEPTION(this->stddbg)
}

void cBase::quitModules(){
	try{
		for (uint i=(uint)modules.size()-1; i>0;i--){
			modules[i]->stop();
		}
		for (uint i=(uint)modules.size()-1; i>0;i--){
			modules[i]->waitForThread();
		}
	}DEFAULT_EXCEPTION(this->stddbg)
}
