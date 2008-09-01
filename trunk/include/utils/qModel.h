#ifndef H_QMODEL
#define H_QMODEL

class qModel {
public:
	virtual void loadModel(const char * fname,const char * param) = 0;
	virtual void prepareMaterials() = 0;
	virtual void updateMaterial(const char * material_name) = 0;
	virtual void drawModel(int mode = 0) = 0;
	virtual void setFrame(const char * obj_name,int num) = 0;
	virtual void setAnimation(const char * obj_name, const char * name) = 0;
	virtual void update() = 0;
};

#endif