#ifndef RUNNABLE_H_
#define RUNNABLE_H_

#define TS_RUNNING	0x00000001
#define TS_RUNNING2	0x00000002
#define TS_POSTINIT	0x00000010
#define TS_FINISHED	0x00000020

class cRunnable{
protected:
	int status;
public:
	cRunnable(){
		status = 0;
	}
	virtual void init() {};		
	virtual void run() {};
	virtual void stop() {};
	inline int getStatus(){
		return status;
	}
	inline void setStatus(int status){
		this->status = status;
	}
};

#endif /*RUNNABLE_H_*/
