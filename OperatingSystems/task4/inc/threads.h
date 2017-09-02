#ifndef __THREADS_H__
#define __THREADS_H__

#include <stdint.h>

#include <list.h>

enum thread_state {
	THREAD_IDLE,
	THREAD_ACTIVE,
	THREAD_BLOCKED,
	THREAD_FINISHING,
	THREAD_FINISHED
};

struct thread {
	struct list_head ll;
	enum thread_state state;
	unsigned long long time;
	struct page *stack;
	uintptr_t stack_addr;
	int stack_order;
	uintptr_t stack_ptr;
};

struct thread *__thread_create(void (*fptr)(void *), void *arg, int order);
struct thread *thread_create(void (*fptr)(void *), void *arg);
void thread_destroy(struct thread *thread);

struct thread *thread_current(void);
void thread_activate(struct thread *thread);
void thread_join(struct thread *thread);
void thread_exit(void);

void disable_preempt(void);
void enable_preempt(void);

void schedule(void);
void force_schedule(void);
void idle(void);

void threads_setup(void);

#endif /*__THREADS_H__*/
