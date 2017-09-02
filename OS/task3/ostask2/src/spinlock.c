#include <spinlock.h>
#include <stdatomic.h>


void spin_init(spinlock_t* lock)
{
    lock->next = 0;
    lock->current = 0;
}

void spin_lock(spinlock_t* lock)
{
    const unsigned ticket = atomic_fetch_add_explicit(&lock->next, 1,
            memory_order_relaxed);

    while (atomic_load_explicit(&lock->current,
                memory_order_acquire) != ticket)
        ;
}


void spin_unlock(spinlock_t* lock)
{
    const unsigned current = atomic_load_explicit(&lock->current,
            memory_order_relaxed);

    atomic_store_explicit(&lock->current, current + 1,
            memory_order_release);
}
