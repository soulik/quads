#include <iostream>
#include <string>
//using namespace std;
#include <stdio.h>
#include <stdarg.h>
#include "errors/Exception.h"

cException::cException(char * text,...){
	char buffer[MAX_E_TEXT_SIZE];
	memset(buffer,0,MAX_E_TEXT_SIZE);

	va_list ap;
	if (text==NULL) return;
	va_start(ap,text);
	vsprintf(buffer,text,ap);
	va_end(ap);

	string = new char[strlen(buffer)+1];
	strcpy(string, buffer);
}

const char * cException::what() const throw(){
	return string;
}
