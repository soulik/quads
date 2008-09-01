#include "utils/qUtils.h"
#include <malloc.h>
#include <string.h>

char * qUtils::safe_strcpy(char * dst, const char * src){
	if (!src) return dst;
	if (!dst){
		dst = (char*) malloc(strlen(src)+1);
	}

	if ((sizeof dst) <(strlen(src)+1)){
		dst = (char*) realloc(dst,strlen(src)+1);
	}
	if (dst) strcpy(dst,src);
	return dst;
}