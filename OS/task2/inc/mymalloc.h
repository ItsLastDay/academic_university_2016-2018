#ifndef __MYMALLOC_H__
#define __MYMALLOC_H__

#include <stddef.h>

void* mymalloc(size_t);
void myfree(void*);
void init_my_malloc();

#endif
