#ifndef __MUTEX_H__
#define __MUTEX_H__

#include <spinlock.h>
#include <list.h>


struct mutex {
	struct spinlock lock;
	struct list_head waitqueue;
	int locked;
};

void mutex_setup(struct mutex *mtx);
void mutex_lock(struct mutex *mtx);
void mutex_unlock(struct mutex *mtx);

#endif /*__MUTEX_H__*/
