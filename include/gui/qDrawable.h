#ifndef H_QDRAWABLE
#define H_QDRAWABLE

#include "runnable.h"
#include "cChild.h"

#define QF_VISIBLE		0x00000001

#define QF_WINDOW		0x00000001

class cChild;

class qDrawable : public cChild{
protected:
	unsigned int id;
	unsigned int flags;
public:
	qDrawable();
	~qDrawable();
	virtual int draw();
	unsigned int getID();
	void setFlags(unsigned int flags);
	unsigned int getFlags();
	unsigned int type;
};

#endif