#include "Thread.h"

cThread::cThread(){
	threadObj = NULL;
	thread = NULL;
}

cThread::cThread(cRunnable * ptr){
	threadObj = ptr;
	thread = NULL;
}

void cThread::start(){
	thread = SDL_CreateThread(threadProc,threadObj);
	//printf("Thread \"%s\" started()\n",getName());
}

SDL_Thread * cThread::getThread(){
	return thread;
}

void cThread::setRunnable(cRunnable * ptr){
	threadObj = ptr;
}
