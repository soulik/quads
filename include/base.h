#ifndef H_BASE
#define H_BASE

#include <vector>
using namespace std;

#include "common.h"
#include "module.h"

class cModule;

class cBase {
protected:
	FILE * stddbg;
	vector<cModule*> modules;
	int status;
public:
	cBase();
	~cBase();
	void initModules();
	void runModules();
	void quitModules();
	inline const cModule * getModule(uint i){
		return modules[i];
	}
	inline int getStatus(){
		return status;
	}
	inline void setStatus(int status){
		this->status = status;
	}
};

#endif