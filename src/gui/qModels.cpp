#include "gui/qModels.h"
#include "errors/errors.h"
#include "errors/exception.h"
#include <stdio.h>
#include "utils/lmem.h"

qModels::qModels(){
	setName("qModels");
	models.clear();
#ifndef NO_MUTEX
	models_mutex = SDL_CreateMutex();
#endif
}

qModels::~qModels(){
	map<string,qModel*>::iterator iter;
	for( iter = models.begin(); iter != models.end(); iter++ ) {
		if (iter->second) delete iter->second;
	}
	models.clear();
#ifndef NO_MUTEX
	SDL_DestroyMutex(models_mutex);
#endif
}

void qModels::loadModel(const char * fname,int type, const char * param){
	try{
		setMutex();
		switch(type){
			case 1:
				loadModelMD3(fname,param);
				break;
			default:
				loadModel3DS(fname,param);
		}
		unsetMutex();
	}catch(cException e){
		throw(e);
	}
}

qModel * qModels::getModel(const char *name){
	map<string,qModel *>::iterator temp;
	temp = models.find(name);
	if( temp == models.end() ) {
		return NULL;
	}
	return temp->second;
}

void qModels::setMutex(){
#ifndef NO_MUTEX
	SDL_mutexP(models_mutex);
#endif
}

void qModels::unsetMutex(){
#ifndef NO_MUTEX
	SDL_mutexV(models_mutex);
#endif
}

void qModels::updateMaterial(const char * model_name, const char * material_name){
	qModel * model = getModel(model_name);
	if (model){
		model->updateMaterial(material_name);
	}
}

void qModels::drawModel(const char * name, int mode){
	qModel * model = getModel(name);
	if (model){
		model->drawModel(mode);
	}
}

void qModels::loadModel3DS( const char * fname, const char * param ){
	c3ds * model = new c3ds();
	model->setDebug(this);
	model->loadModel(fname,param);
	models.insert(make_pair(fname,model));
	dprintf("Model '%s' successfully loaded with %d objects\n",fname,model->objects.size());
}

void qModels::loadModelMD3( const char * fname, const char * param ){
	cMD3 * model = new cMD3();
	model->setDebug(this);
	model->loadModel(fname,param);
	models.insert(make_pair(fname,model));
	dprintf("Model '%s' successfully loaded\n",fname);
}