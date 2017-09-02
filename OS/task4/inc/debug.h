#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <print.h>
#include <ints.h>

#define BUG(...) \
	do { \
		disable_ints(); \
		printf("BUG at %s:%d: ", __FILE__, __LINE__); \
		printf(__VA_ARGS__); \
		while (1) \
			__asm__ ("hlt"); \
	} while (0);

#define BUG_ON(cond) \
	if (cond) { \
		BUG("\""#cond"\"\n"); \
	}

#endif /*__DEBUG_H__*/
