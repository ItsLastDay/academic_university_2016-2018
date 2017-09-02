#ifndef __SPINLOCK_H__
#define __SPINLOCK_H__

#include <stdatomic.h>

typedef struct __spinlock_t
{
    atomic_uint next;
    atomic_uint current;
} spinlock_t;


void spin_init(spinlock_t* lock);
void spin_lock(spinlock_t* lock);
void spin_unlock(spinlock_t* lock);

#endif
