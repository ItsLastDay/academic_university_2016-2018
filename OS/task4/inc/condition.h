#ifndef __CONDITION_VARIABLE_H__
#define __CONDITION_VARIABLE_H__

#include <spinlock.h>
#include <list.h>


struct condition {
	struct spinlock lock;
	struct list_head waitqueue;
};

void condition_setup(struct condition *cond);

struct mutex;

void condition_wait(struct condition *cond, struct mutex *mtx);
void condition_signal(struct condition *cond);
void condition_broadcast(struct condition *cond);

#endif /*__CONDTION_VARIABLE_H__*/
