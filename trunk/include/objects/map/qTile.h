#ifndef H_QTILE
#define H_QTILE

#include "common.h"
#include "objects/qPoint.h"
#include "objects/map/qBox.h"

class qTile;
class qBox;

/*
	Surrounding tiles numbers:

	7	8	9
	4	(5)	6
	1	2	3

	- where 5 is self reference - "this"

*/

struct qSurrounding {
	qTile * N;
	qTile * NE;
	qTile * E;
	qTile * SE;
	qTile * S;
	qTile * SW;
	qTile * W;
	qTile * NW;
};

class qTile {
public:
	long x,y,z;
	unsigned long flags;
	//qSurrounding s;
	qBox * parent;
	qTile();
};

#endif