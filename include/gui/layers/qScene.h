#ifndef H_QSCENE
#define H_QSCENE

#include "gui/qDrawable.h"
#include "gui/qModels.h"

class qScene : public qDrawable, public qModels{
private:
	qDrawable * heightMap;
public:
	qScene();
	~qScene();
	int draw();
	inline void setParent(cBase * parent);
};

#endif