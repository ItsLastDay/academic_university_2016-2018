#ifndef __SPINLOCK_H__
#define __SPINLOCK_H__

struct spinlock {
	/* Well as you can see, it's not true SPINlock. */
	void *dummy;
};

void spin_setup(struct spinlock *lock);

void spin_lock(struct spinlock *lock);
void spin_unlock(struct spinlock *lock);

/* These two required to guard data that might be updated
 * from the interrupt handler, so in this course we don't
 * actually need them at all. */
int spin_lock_irqsave(struct spinlock *lock);
void spin_unlock_irqrestore(struct spinlock *lock, int irqs);

#endif /*__SPINLOCK_H__*/
