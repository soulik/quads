#ifndef H_QOBJECTS
#define H_QOBJECTS

#include "gui/qDrawable.h"

class qObjects : public qDrawable{
private:
	int dList[10];
public:
	qObjects();
	int draw();
};

#endif