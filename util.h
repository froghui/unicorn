#ifndef UTIL_H
#define UTIL_H


typedef struct unicorn_s unicorn_t;
struct unicorn_s
{
	int parent_pid;
	int child_pid;
	char * unicorn_id;
};

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
if(result < 0 && errCode != EEXIST){ \
	printf("%s get error return %d and msg is %s \n", message, result,  strerror(errCode)); \
	exit(errCode); \
} \


#endif

