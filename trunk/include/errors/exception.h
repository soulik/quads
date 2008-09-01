#ifndef EXCEPTION_H_
#define EXCEPTION_H_

#include <stdio.h>
#include <exception>

#define STREAM_STDDBG	stdout
#define MAX_E_TEXT_SIZE	1024

using namespace std;

class cException: public exception{
private:
	char * string;
public:
	cException(char * text,...);
	
	virtual const char* what() const throw();
};

#define DEFAULT_EXCEPTION(STREAM)	catch (cException e){\
		fprintf((STREAM),"Exception: %s\n",e.what());\
		fflush((STREAM));\
	}\


#endif /*EXCEPTION_H_*/
