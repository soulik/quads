#include "modules/qTexturer.h"
#include "errors/exception.h"

qTexturer::qTexturer(cBase * parent): cModule(parent){
#ifndef NO_MUTEX
	tex_mutex = SDL_CreateMutex();
#endif
	setName("qTexturer");
	base_dir = (char*)malloc(512);
#ifdef DEBUGGING
	dprintf("qTexturer()\n");
#endif
}

qTexturer::~qTexturer(){
	for (unsigned int i=0; i<images.size() ; i++){
		delete images[i];
	}
	free(base_dir);
	images.clear();
#ifndef NO_MUTEX
	SDL_DestroyMutex(tex_mutex);
#endif
}

void qTexturer::setBaseDir(const char * dir){
	if (dir){
		strcpy(base_dir,dir);
	}else{
		dprintf("Tried to set NULL base dir\n");
	}
}

unsigned int qTexturer::addImage(float width, float height, int flags, const char * name){
#ifndef NO_MUTEX
	SDL_mutexP(tex_mutex);
#endif
	qImage * img = new qImage();
	img->createEmpty(width, height,flags);
	images.push_back(img);
	image_names[name] = images.size()-1;
#ifndef NO_MUTEX
	SDL_mutexV(tex_mutex);
#endif
	return (uint)images.size();
}

unsigned int qTexturer::loadImage(const char *fname, unsigned int flags){
#ifndef NO_MUTEX
	SDL_mutexP(tex_mutex);
#endif
	if (!fname) return 0;
	qImage * img = new qImage();
	char temp[512];
	strcpy(temp,base_dir);
	if (img->load(strcat(temp,fname),flags)!=1) return 0;
	images.push_back(img);
	image_names[fname] = images.size()-1;
#ifndef NO_MUTEX
	SDL_mutexV(tex_mutex);
#endif
	return (uint)images.size();
}

qImage * qTexturer::getImage(unsigned int id){
#ifndef NO_MUTEX
	SDL_mutexP(tex_mutex);
#endif
	if (id>=images.size()) return 0;
	if (!images[id]) throw (cException("Image NULL"));
#ifndef NO_MUTEX
	SDL_mutexV(tex_mutex);
#endif
	//temporary solution
	while (!images[id]->state){
		SDL_Delay(1);
	}
	return images[id];
}

qImage * qTexturer::getImage(const char * name){
#ifndef NO_MUTEX
	SDL_mutexP(tex_mutex);
#endif
	if (!name) return 0;
	size_t id=0;
	map<string, uint>::iterator found = image_names.find(name);
	if (found != image_names.end()){
		id = found->second;
	}else throw (cException("Can't find image"));
	if (!images[id]) throw (cException("Image NULL"));
#ifndef NO_MUTEX
	SDL_mutexV(tex_mutex);
#endif
	//temporary solution
	while (!images[id]->state){
		SDL_Delay(1);
	}
	return images[id];
}

void qTexturer::init(){
	status = TS_RUNNING;
}

void qTexturer::stop(){
	status = 0;
}