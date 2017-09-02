#include <condition.h>
#include <threads.h>
#include <mutex.h>


void condition_setup(struct condition *cond)
{
	spin_setup(&cond->lock);
	list_init(&cond->waitqueue);
}

struct wait {
	struct list_head ll;
	struct thread *thread;
};

void condition_wait(struct condition *cond, struct mutex *mtx)
{
	struct thread *self = thread_current();
	struct wait node;

	const int enable = spin_lock_irqsave(&cond->lock);

	node.thread = self;
	self->state = THREAD_BLOCKED;
	list_add_tail(&node.ll, &cond->waitqueue);
	mutex_unlock(mtx);

	spin_unlock_irqrestore(&cond->lock, enable);

	schedule();
	mutex_lock(mtx);
}

void condition_signal(struct condition *cond)
{
	const int enable = spin_lock_irqsave(&cond->lock);

	if (!list_empty(&cond->waitqueue)) {
		struct wait *node = (struct wait *)list_first(&cond->waitqueue);

		list_del(&node->ll);
		thread_activate(node->thread);
	}
	spin_unlock_irqrestore(&cond->lock, enable);
}

void condition_broadcast(struct condition *cond)
{
	const int enable = spin_lock_irqsave(&cond->lock);
	struct list_head list;

	list_init(&list);
	list_splice(&cond->waitqueue, &list);
	spin_unlock_irqrestore(&cond->lock, enable);

	struct list_head *head = &list;
	struct list_head *ptr = head->next;

	while (ptr != head) {
		struct wait *node = (struct wait *)ptr;

		ptr = ptr->next;
		thread_activate(node->thread);
	}
}
