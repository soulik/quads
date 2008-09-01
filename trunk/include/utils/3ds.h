#ifndef H_3DS
#define H_3DS

#include "common.h"
#include "qDebug.h"
#include "utils/3ds_defs.h"
#include "utils/qModel.h"
#include <vector>
#include <map>
#include <string>
#include <stdio.h>

using namespace std;

struct Chunk {
	ushort	id;
	long	length;
};

class c3ds : public qModel{
private:
	FILE * f;
	int readChunk(Chunk * chunk);
	void readChunks();
	void skipChunk(Chunk * chunk);
	void readMain(Chunk * chunk);
	void read3DEditor(Chunk * chunk);
	void readObject(Chunk * chunk);
	void readTriangularMesh(Chunk * chunk, xObject * co);
	void readMaterial(Chunk * chunk);
	void readFacesDescription(Chunk * chunk, xObject * co);
	void readMap(Chunk * chunk, xMap * cmap);
	void readColor(Chunk * chunk, xMaterial * cm, char mode);
	void readRGBb(xColor * color);
	void readRGBf(xColor * color);
	void readString(char * text, int max_len);
	qDebug * debug;
	void drawObject(xObject * object, int mode = 0);
public:
	vector<xObject*> objects;
	unsigned long version;
	unsigned long mesh_version;
	map<string,xMaterial*> material;
	c3ds();
	~c3ds();
	void setDebug(qDebug * debug);
	void loadModel(const char * fname,const char * param=NULL);
	void prepareMaterials();
	void updateMaterial(const char * material_name);
	void drawModel(int mode = 0);
	void setFrame(const char * obj_name,int num);
	void setAnimation(const char * obj_name,const char * name);
	void update();

};

#endif