#ifndef H_MODULE
#define H_MODULE

#include "thread.h"
#include "runnable.h"
#include "cChild.h"
#include "qDebug.h"

class cModule: public cThread, public qDebug, public cRunnable, public cChild {
protected:
	int threaded;
	SDL_mutex * mutex;

public:
	cModule(cBase * parent): cChild(parent){
		threaded=0;
#ifndef NO_MUTEX
		mutex = SDL_CreateMutex();
#endif
	}
	~cModule();
	inline int hasThread(){
		return threaded;
	}
	inline void preRun(){
		if (threaded) start();
	}
	virtual void postRun(){}
	void sendMessage(char * msg);
	void lockMutex();
	void unlockMutex();

};

#endif