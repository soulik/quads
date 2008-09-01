#ifndef THREAD_H_
#define THREAD_H_

#include <SDL/SDL.h>
#include "Runnable.h"
#include "errors/exception.h"

class cThread {
public:
	cThread();
	cThread(cRunnable * ptr);
	void start();
	SDL_Thread * getThread();
	void setRunnable(cRunnable * ptr);

	int waitForThread(){
		return waitForThread(this);
	}
	static int waitForThread(cThread * cthread){
		if (!cthread) return -1;
		int result;
		SDL_WaitThread(cthread->getThread(),&result);
		return result;
	}
protected:
	cRunnable * threadObj;
	SDL_Thread * thread;
	static int __cdecl threadProc(void * ptr){
		try{
			if (!ptr) throw cException("Thread main function NULL");
			((cRunnable*)ptr)->run();
		}DEFAULT_EXCEPTION(stderr)
		return 0;
	}
};

#endif /*THREAD_H_*/
