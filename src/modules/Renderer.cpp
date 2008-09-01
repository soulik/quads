#include "main.h"
#include "game/gamelogic.h"
#include <math.h>
#include "modules/renderer.h"
#include "modules/game.h"
#include "modules/qTexturer.h"
#include "errors/exception.h"

// *** layers ***
#include "gui/layers/qHMap.h"
#include "gui/layers/qObjects.h"
#include "gui/layers/qScene.h"

#define STDDBG (stderr)

cRenderer::cRenderer(cBase * parent): cGui(parent){
	//drawer = NULL;
	main_counter  = 0;
	this->setRunnable(this);
	setName("cRenderer");
	threaded=1;
	font = NULL;
	frames = 0;
	fps = 0.0f;
	time = 0;
#ifndef NO_MUTEX
	rend_mutex = SDL_CreateMutex();
#endif
#ifdef DEBUGGING
	dprintf("cRenderer()\n");
#endif
}

cRenderer::~cRenderer(){
#ifndef NO_MUTEX
	SDL_DestroyMutex(rend_mutex);
#endif
}

void cRenderer::init(){
	try{
	}DEFAULT_EXCEPTION(STDDBG)
}

void cRenderer::init(int width, int height,int red_size, int green_size, int blue_size, int depth_size, int doublebuffer){
	this->scr_width = width;
	this->scr_height = height;
	this->red_size = red_size;
	this->green_size = green_size;
	this->blue_size = blue_size;
	this->depth_size = depth_size;
	this->doublebuffer = doublebuffer;

	status = TS_RUNNING2;
}

void cRenderer::run(){
#ifndef NO_MUTEX
	SDL_mutexP(rend_mutex);
#endif
	while (status != TS_RUNNING2){
		SDL_Delay(1);
	}
	dprintf("run()\n");
	//OpenGL must be initialized inside thread....

	ssGL.initGL(scr_width,scr_height,red_size, green_size, blue_size, depth_size, doublebuffer);
	initLayers();
		
	GAME->glogic->postInit();
	RENDER->font = new qFont();

	SCRIPT->call("renderer_ready");

#ifndef NO_MUTEX
	SDL_mutexP(TEXTURE->tex_mutex);
#endif
	SCRIPT->call("startup");

#ifndef NO_MUTEX
	SDL_mutexV(TEXTURE->tex_mutex);
#endif

	status |= TS_POSTINIT;

	GLenum err=GL_NO_ERROR;
	status |= TS_RUNNING;
	dprintf("SDL_Error() returns: \"%s\"\n",SDL_GetError());
	try{
		while(status&TS_RUNNING){
			status &= ~TS_FINISHED;
			GAME->processEvents();
			drawScene();
			//err = glGetError();
			countFPS();
			SDL_GL_SwapBuffers( );
			//SDL_Delay(1);
			status |= TS_FINISHED;
		}
	}DEFAULT_EXCEPTION(STDDBG)
	SCRIPT->call("renderer_end");

	quitLayers();
	ssGL.guitGL();

#ifndef NO_MUTEX
	SDL_mutexV(rend_mutex);
#endif
	dprintf("Done\n");
}

void cRenderer::stop(){
	status &= ~TS_RUNNING;
}

void cRenderer::prepareMultitexture(){
#ifndef NO_MUTEX
	SDL_mutexP(TEXTURE->tex_mutex);
#endif



#ifndef NO_MUTEX
	SDL_mutexV(TEXTURE->tex_mutex);
#endif
}

void cRenderer::prepareScene(){
#ifndef NO_MUTEX
	SDL_mutexP(TEXTURE->tex_mutex);
#endif
	__SCRIPT->call("prepareScene");
#ifndef NO_MUTEX
	SDL_mutexV(TEXTURE->tex_mutex);
#endif
}

void cRenderer::prepareTexture(){
#ifndef NO_MUTEX
	SDL_mutexP(TEXTURE->tex_mutex);
#endif
	__SCRIPT->call("updateTextures");
#ifndef NO_MUTEX
	SDL_mutexV(TEXTURE->tex_mutex);
#endif
}

void cRenderer::prepareOrthogonal(){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho( 0, scr_width, scr_height,0,-1,1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void cRenderer::drawPlayerView(int i){
	if (i){
		GAME->glogic->camera->setup2();
	}else{
		GAME->glogic->camera->setup();
	}
}

void cRenderer::drawScene(){
	if(GAME->glogic&&(status & TS_RUNNING)){
		//prepareTexture();
		prepareScene();
		drawPlayerView();
		drawLayers();
	}
}

void cRenderer::initLayers(){
	dObjects.push_back(new qScene());
	initWindows();
	for (unsigned int i=0; i<dObjects.size() ; i++){
		dObjects[i]->setParent(parent);
	}
	dprintf("%d layers loaded\n",dObjects.size());
}

void cRenderer::quitLayers(){
	for (unsigned int i=0; i<dObjects.size() ; i++){
		delete dObjects[i];
	}
}

void cRenderer::drawLayers(){
	for (unsigned int i=0; i<dObjects.size() ; i++){
		dObjects[i]->draw();
	}

}

void cRenderer::countFPS(){
	frames++;
	if (frames%10==0){
		//fps = 10.0f/float(time)*1000.0f;
		float delta_time = float(SDL_GetTicks()-time);
		fps = (delta_time>0) ? (1000.0f*10.0f/delta_time) : 666.0f;
		time = SDL_GetTicks();
	}

}