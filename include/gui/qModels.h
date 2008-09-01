#ifndef H_QMODELS
#define H_QMODELS

#include "utils/qModel.h"
#include "utils/3ds.h"
#include "utils/md3.h"
#include "qDebug.h"
#include <sdl/SDL.h>

#include <string>
#include <map>

using namespace std;

class qModels:public qDebug {
private:
	map <string, qModel*> models;
protected:
	SDL_mutex * models_mutex;
public:
	qModels();
	~qModels();
	void loadModel(const char * fname, int type=0, const char * param = NULL);
	void loadModel3DS(const char * fname, const char * param);
	void loadModelMD3(const char * fname, const char * param);
	qModel * getModel(const char * name);
	void setMutex();
	void unsetMutex();
	void updateMaterial(const char * model_name, const char * material_name);
	void drawModel(const char * name, int mode = 0);
};

#endif