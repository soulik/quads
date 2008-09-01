#ifndef H_QHMAP
#define H_QHMAP

#include "gui/qDrawable.h"
#include "objects/qVector.h"

class qHMap : public qDrawable{
public:
	int draw();
	void drawHMapPoints(qVector pos);
	void drawHMapPointsAroundPlayer(qVector pos);
};


#endif