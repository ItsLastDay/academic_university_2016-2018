#include <mutex.h>
#include <threads.h>
#include <debug.h>


void mutex_setup(struct mutex *mtx)
{
	spin_setup(&mtx->lock);
	list_init(&mtx->waitqueue);
	mtx->locked = 0;
}

struct wait {
	struct list_head ll;
	struct thread *thread;
};

void mutex_lock(struct mutex *mtx)
{
	const int enable = spin_lock_irqsave(&mtx->lock);

	if (mtx->locked) {
		struct thread *self = thread_current();
		struct wait node;

		node.thread = self;
		self->state = THREAD_BLOCKED;
		list_add_tail(&node.ll, &mtx->waitqueue);
		spin_unlock_irqrestore(&mtx->lock, enable);
		schedule();
		return;
	}

	mtx->locked = 1;
	spin_unlock_irqrestore(&mtx->lock, enable);
}

void mutex_unlock(struct mutex *mtx)
{
	const int enable = spin_lock_irqsave(&mtx->lock);

	if (list_empty(&mtx->waitqueue)) {
		mtx->locked = 0;
		spin_unlock_irqrestore(&mtx->lock, enable);
		return;
	}

	struct wait *next = (struct wait *)list_first(&mtx->waitqueue);

	list_del(&next->ll);
	thread_activate(next->thread);
	spin_unlock_irqrestore(&mtx->lock, enable);
}
