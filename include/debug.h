#ifndef __DEBUG_H__
#define __DEBUG_H__

	#include <stdio.h>
	#include <assert.h>


	#define test(cond, ...)                         \
		do {                                        \
			if(!(cond)) {                           \
				fflush(stdout);                     \
				fprintf(stderr, "\33[1;31m");       \
				fprintf(stderr, __VA_ARGS__);       \
				fprintf(stderr, "\33[0m\n");        \
				assert(0);                          \
			}                                       \
		} while(0)


	#define Log(format,...)                                                                                       \
	 	fprintf(stdout, "\33[1;34m%s,%s,%d: " format "\33[0m\n", __FILE__, __func__, __LINE__, ## __VA_ARGS__),   \
		fflush(stdout)


#endif
