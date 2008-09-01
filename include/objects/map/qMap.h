#ifndef H_QMAP
#define H_QMAP
#include "common.h"
#include "qDebug.h"
#include "objects/qQuad.h"
#include "objects/map/qTile.h"
#include <vector>

using namespace std;

struct qMapHeader{
	ulong width, height;
	uchar unused[32];
};

struct qMapTile{
	long x,y,z;
	uchar unused[32];
};

class qMap: public qDebug{
private:
	ulong width,height;
	qTile * map;
	qTile temp;
public:
	qMap();
	~qMap();

	inline ulong getWidth() {return width;}
	inline ulong getHeight() {return height;}
	unsigned long flags;
	vector<qQuad *> worldMap;

	void LoadTestMap(int width, int height);
	void LoadMap(const char * fname);
	void LoadMapV2(const char * fname);
	void SaveMapV2(const char * fname);
	void GenerateMapV2(long width, long height, float t=0.0f);
	void ClearMap(long width=0, long height=0);
	qTile * GetMap(ulong x, ulong y);
/*
	unsigned int GetMapFlag(int x, int y);
	void SetMapFlag(int x, int y, unsigned int flag);
*/
};

#endif