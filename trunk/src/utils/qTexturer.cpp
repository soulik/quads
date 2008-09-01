#include "utils/qTexturer.h"

qTexturer::qTexturer(cBase * parent): cModule(parent){
	tex_mutex = SDL_CreateMutex();
}

qTexturer::~qTexturer(){
	for (unsigned int i=0; i<images.size() ; i++){
		delete images[i];
	}
	images.clear();
	SDL_DestroyMutex(tex_mutex);
}

unsigned int qTexturer::addImage(float width, float height){
	SDL_mutexP(tex_mutex);
	qImage * img = new qImage();
	img->createEmpty(width, height);
	images.push_back(img);
	SDL_mutexV(tex_mutex);
	return images.size();
}

unsigned int qTexturer::loadImage(const char *fname, unsigned int flags){
	SDL_mutexP(tex_mutex);
	if (!fname) return 0;
	qImage * img = new qImage();
	if (img->load(fname,flags)!=1) return 0;
	images.push_back(img);
	SDL_mutexV(tex_mutex);
	return images.size();
}

qImage * qTexturer::getImage(unsigned int id){
	SDL_mutexP(tex_mutex);
	if (id>=images.size()) return 0;
	SDL_mutexV(tex_mutex);
	return images[id];
}