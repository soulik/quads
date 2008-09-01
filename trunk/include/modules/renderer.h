#ifndef RENDERER_H_
#define RENDERER_H_

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include "module.h"
#include "gui/qFont.h"
#include "objects/qVector.h"
#include "utils/qRGBA.h"
#include "gui/gui.h"
#include "gui/cGL.h"

class cRenderer : public cGui{
private:
	int scr_width;
	int scr_height;
	int red_size;
	int green_size;
	int blue_size;
	int depth_size;
	int doublebuffer;

	void initLayers();
	void drawLayers();
	void quitLayers();
	cGL ssGL;
	unsigned int frames;
	unsigned int time;
	float fps;
public:
	qFont * font;
	SDL_mutex * rend_mutex;
	
	unsigned int main_counter;
	
	cRenderer(cBase * parent);
	virtual ~cRenderer();
	
	void init(int width,int height,int red_size=8, int green_size=8, int blue_size=8, int depth_size=16, int doublebuffer=1);
	void drawPlayerView(int i=0);
	void drawScene();
	void prepareScene();
	void prepareTexture();
	void prepareMultitexture();
	void prepareOrthogonal();
	void countFPS();
	inline SDL_Surface * getSurface(){
		return ssGL.surface;
	}
	inline float getFPS(){
		return fps;
	};
	
	void init();
	void stop();
	void run();
	
};

#endif /*RENDERER_H_*/
