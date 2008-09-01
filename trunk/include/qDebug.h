#ifndef H_QDEBUG
#define H_QDEBUG

#include <SDL/sdl.h>
#include "named.h"

class qDebug : public cNamed{
private:
	SDL_mutex * mutex;
public:
	qDebug();
	~qDebug();
	void dprintf(const char * fmt, ...);
};

#endif