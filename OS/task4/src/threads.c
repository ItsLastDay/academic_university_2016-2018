#include <stdatomic.h>
#include <stdint.h>

#include <threads.h>
#include <string.h>
#include <alloc.h>
#include <time.h>
#include <debug.h>


struct thread_switch_frame {
	uint64_t rflags;
	uint64_t r15;
	uint64_t r14;
	uint64_t r13;
	uint64_t r12;
	uint64_t rbx;
	uint64_t rbp;
	uint64_t rip;
} __attribute__((packed));


static const size_t STACK_ORDER = 1;
static const unsigned long long SLICE = 10;

static struct mem_cache thread_cache;
static struct thread *current;
static struct thread *idle_thread;
static struct list_head runqueue;
static struct spinlock runqueue_lock;
static atomic_int preempt_cnt;


static struct thread *thread_alloc(void)
{
	return mem_cache_alloc(&thread_cache);
}

static void thread_free(struct thread *thread)
{
	mem_cache_free(&thread_cache, thread);
}

void disable_preempt(void)
{
	atomic_fetch_add_explicit(&preempt_cnt, 1, memory_order_relaxed);
}

void enable_preempt(void)
{
	atomic_fetch_sub_explicit(&preempt_cnt, 1, memory_order_relaxed);
}

void threads_setup(void)
{
	const size_t size = sizeof(struct thread);
	const size_t align = 64;

	mem_cache_setup(&thread_cache, size, align);
	spin_setup(&runqueue_lock);
	list_init(&runqueue);
	current = thread_alloc();
	BUG_ON(!current);
	memset(current, 0, sizeof(current));
	current->state = THREAD_ACTIVE;
}

static void place_thread(struct thread *next)
{
	struct thread *prev = thread_current();

	if (prev->state == THREAD_FINISHING)
		prev->state = THREAD_FINISHED;

	spin_lock(&runqueue_lock);
	if (prev->state == THREAD_ACTIVE)
		list_add_tail(&prev->ll, &runqueue);
	spin_unlock(&runqueue_lock);

	current = next;
	next->time = current_time();
}

void thread_entry(struct thread *thread, void (*fptr)(void *), void *arg)
{
	place_thread(thread);
	enable_ints();

	fptr(arg);
	thread_exit();
}

struct thread *__thread_create(void (*fptr)(void *), void *arg, int order)
{
	void __thread_entry(void);

	const size_t stack_size = (size_t)1 << (PAGE_SHIFT + order);
	struct page *stack = __page_alloc(order);

	if (!stack)
		return 0;

	struct thread *thread = thread_alloc();
	struct thread_switch_frame *frame;

	thread->stack = stack;
	thread->stack_order = order;
	thread->stack_addr = (uintptr_t)va(page_addr(stack));
	thread->stack_ptr = thread->stack_addr + stack_size - sizeof(*frame);
	thread->state = THREAD_BLOCKED;

	frame = (struct thread_switch_frame *)thread->stack_ptr;
	frame->r12 = (uintptr_t)thread;
	frame->r13 = (uintptr_t)fptr;
	frame->r14 = (uintptr_t)arg;
	frame->rbp = 0;
	frame->rflags = (1ul << 2);
	frame->rip = (uintptr_t)__thread_entry;
	return thread;
}

struct thread *thread_create(void (*fptr)(void *), void *arg)
{
	return __thread_create(fptr, arg, STACK_ORDER);
}

void thread_destroy(struct thread *thread)
{
	__page_free(thread->stack, thread->stack_order);
	thread_free(thread);
}

struct thread *thread_current(void)
{ return current; }

void thread_activate(struct thread *thread)
{
	const int enable = spin_lock_irqsave(&runqueue_lock);

	thread->state = THREAD_ACTIVE;
	list_add_tail(&thread->ll, &runqueue);
	spin_unlock_irqrestore(&runqueue_lock, enable);
}

void thread_exit(void)
{
	struct thread *self = thread_current();

	self->state = THREAD_FINISHING;
	while (1)
		force_schedule();
}

void thread_join(struct thread *thread)
{
	while (thread->state != THREAD_FINISHED)
		force_schedule();
}

static void thread_switch_to(struct thread *next)
{
	void __thread_switch(uintptr_t *prev_state, uintptr_t next_state);

	struct thread *self = thread_current();

	__thread_switch(&self->stack_ptr, next->stack_ptr);
	place_thread(self);
}

static void __schedule(int force)
{
	const unsigned long long time = current_time();
	struct thread *prev = thread_current();
	struct thread *next = idle_thread;

	if (!force && prev->state == THREAD_ACTIVE && time - prev->time < SLICE)
		return;

	spin_lock(&runqueue_lock);
	if (!list_empty(&runqueue)) {
		next = (struct thread *)list_first(&runqueue);
		list_del(&next->ll);
	}
	spin_unlock(&runqueue_lock);

	if (prev == next)
		return;

	if (next && (next != idle_thread || prev->state != THREAD_ACTIVE)) {
		BUG_ON(!next);
		thread_switch_to(next);
	}
}

void schedule(void)
{
	if (atomic_load_explicit(&preempt_cnt, memory_order_relaxed))
		return;

	const int enable = ints_enabled();

	disable_ints();
	__schedule(0);

	if (enable) enable_ints();
}

void force_schedule(void)
{
	BUG_ON(atomic_load_explicit(&preempt_cnt, memory_order_relaxed));

	const int enable = ints_enabled();

	disable_ints();
	__schedule(1);

	if (enable) enable_ints();
}

void idle(void)
{
	struct thread *self = thread_current();

	BUG_ON(idle_thread);
	idle_thread = self;
	self->state = THREAD_IDLE;
	while (1)
		force_schedule();
}
