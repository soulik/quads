#ifndef H_CCHILD
#define H_CCHILD

#include <stdlib.h>

class cBase;

class cChild {
protected:
	cBase * parent;
	friend cBase;
public:
	cChild(){
		parent = NULL;
	}
	cChild(cBase * parent){
		this->parent = parent;
	}
	inline virtual void setParent(cBase * parent){
		this->parent = parent;
	}
	inline const cBase * getParent(){
		return parent;
	}

};

#endif