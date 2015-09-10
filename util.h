#ifndef UTIL_H
#define UTIL_H


void random_string(char * string, unsigned length);

#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define check_result(value, message) \
if (value < 0){ \
	printf("%s get error: %d %s \n", message, value,  strerror(errno)); \
	exit(value); \
} \

int result;
int errCode;

#define check_mkdir(value, message) \
result = value; \
errCode = errno; \
if(errCode == EEXIST || result==0 ){ \
	                 \
}else{ \
	printf("%s get error: %d %s \n", message, result,  strerror(errCode)); \
	exit(errCode); \
} \


#endif

