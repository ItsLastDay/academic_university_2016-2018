#include <spinlock.h>
#include <threads.h>
#include <ints.h>

void spin_setup(struct spinlock *lock)
{
	/* Since we aren't going to use lock->dummy there is no need
	 * to initialize it. */
	(void) lock;
}

void spin_lock(struct spinlock *lock)
{
	(void) lock;
	disable_preempt();
}

void spin_unlock(struct spinlock *lock)
{
	(void) lock;
	enable_preempt();
}

int spin_lock_irqsave(struct spinlock *lock)
{
	const int enable = ints_enabled();

	disable_ints();
	spin_lock(lock);
	return enable;
}

void spin_unlock_irqrestore(struct spinlock *lock, int enable)
{
	spin_unlock(lock);
	if (enable)
		enable_ints();
}
