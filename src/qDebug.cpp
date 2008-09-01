#include "qDebug.h"

qDebug::qDebug(){
#ifndef NO_MUTEX
	mutex = SDL_CreateMutex();
#endif
}

qDebug::~qDebug(){
#ifndef NO_MUTEX
	SDL_DestroyMutex(mutex);
#endif
}

void qDebug::dprintf(const char * fmt,...){
#ifndef NO_MUTEX
	SDL_mutexP(mutex);
#endif
	char text[1024];
	memset(text,0,1024);
	
	va_list ap;
	if (fmt==NULL) return;
	va_start(ap,fmt);
	vsprintf(text,fmt,ap);
	va_end(ap);
	
	fprintf(stderr,"%-8s \t| %s",getName(),text);
#ifndef NO_MUTEX
	SDL_mutexV(mutex);
#endif
}