#include <serial.h>
#include <memory.h>
#include <balloc.h>
#include <paging.h>
#include <debug.h>
#include <alloc.h>
#include <print.h>
#include <ints.h>
#include <time.h>
#include <threads.h>
#include <mutex.h>
#include <condition.h>
#include <ramfs.h>
#include <initramfs.h>


static void qemu_gdb_hang(void)
{
#ifdef DEBUG
	static volatile int wait = 1;

	while (wait);
#endif
}

static void test_kmap(void)
{
	const size_t count = 1024;
	struct page **pages = mem_alloc(sizeof(*pages) * count);
	size_t i;

	BUG_ON(!pages);
	for (i = 0; i != count; ++i) {
		pages[i] = __page_alloc(0);
		if (!pages[i])
			break;
	}

	char *ptr = kmap(pages, i);

	BUG_ON(!ptr);
	BUG_ON((uintptr_t)ptr < HIGHER_BASE);

	for (size_t j = 0; j != i * PAGE_SIZE; ++j)
		ptr[i] = 13;

	for (size_t j = 0; j != i * PAGE_SIZE; ++j)
		BUG_ON(ptr[i] != 13);

	kunmap(ptr);
	mem_free(pages);
}

static void test_alloc(void)
{
	struct list_head head;
	unsigned long count = 0;

	list_init(&head);
	while (1) {
		struct list_head *node = mem_alloc(sizeof(*node));

		if (!node)
			break;
		BUG_ON((uintptr_t)node < HIGHER_BASE);
		++count;
		list_add(node, &head);
	}

	printf("Allocated %lu bytes\n", count * sizeof(head));

	while (!list_empty(&head)) {
		struct list_head *node = head.next;

		BUG_ON((uintptr_t)node < HIGHER_BASE);
		list_del(node);
		mem_free(node);
	}

	mem_alloc_shrink();
}

static void test_slab(void)
{
	struct list_head head;
	struct mem_cache cache;
	unsigned long count = 0;

	list_init(&head);
	mem_cache_setup(&cache, sizeof(head), sizeof(head));
	while (1) {
		struct list_head *node = mem_cache_alloc(&cache);

		if (!node)
			break;
		BUG_ON((uintptr_t)node < HIGHER_BASE);
		++count;
		list_add(node, &head);
	}

	printf("Allocated %lu bytes\n", count * sizeof(head));

	while (!list_empty(&head)) {
		struct list_head *node = head.next;

		BUG_ON((uintptr_t)node < HIGHER_BASE);
		list_del(node);
		mem_cache_free(&cache, node);
	}

	mem_cache_release(&cache);
}

static void test_buddy(void)
{
	struct list_head head;
	unsigned long count = 0;

	list_init(&head);
	while (1) {
		struct page *page = __page_alloc(0);

		if (!page)
			break;
		++count;
		list_add(&page->ll, &head);
	}

	printf("Allocated %lu pages\n", count);

	while (!list_empty(&head)) {
		struct list_head *node = head.next;
		struct page *page = CONTAINER_OF(node, struct page, ll);

		list_del(&page->ll);
		__page_free(page, 0);
	}
}

static void __th1_main(void *data)
{
	const int id = (int)(uintptr_t)data;

	for (size_t i = 0; i != 5; ++i) {
		printf("i'm %d\n", id);
		force_schedule();
	}
}

static void test_threads(void)
{
	struct thread *th1 = thread_create(&__th1_main, (void *)1);
	struct thread *th2 = thread_create(&__th1_main, (void *)2);

	thread_activate(th1);
	thread_activate(th2);

	thread_join(th2);
	thread_join(th1);

	thread_destroy(th1);
	thread_destroy(th2);
}

static void wait(unsigned long long count)
{
	const unsigned long long time = current_time();

	while (time + count > current_time())
		force_schedule();
}

static void __th2_main(void *data)
{
	struct mutex *mtx = data;

	for (size_t i = 0; i != 5; ++i) {
		mutex_lock(mtx);
		printf("%p acquired mutex\n", thread_current());
		wait(100);
		printf("%p released mutex\n", thread_current());
		mutex_unlock(mtx);
	}
}

static void test_mutex(void)
{
	struct mutex mtx;
	struct thread *th1 = thread_create(&__th2_main, (void *)&mtx);
	struct thread *th2 = thread_create(&__th2_main, (void *)&mtx);

	mutex_setup(&mtx);
	thread_activate(th1);
	thread_activate(th2);

	thread_join(th2);
	thread_join(th1);

	thread_destroy(th1);
	thread_destroy(th2);
}

struct future {
	struct mutex mtx;
	struct condition cond;
	int value;
	int set;
};

static void future_setup(struct future *future)
{
	mutex_setup(&future->mtx);
	condition_setup(&future->cond);
	future->value = 0;
	future->set = 0;
}

static void future_set(struct future *future, int value)
{
	mutex_lock(&future->mtx);
	future->value = value;
	future->set = 1;
	condition_broadcast(&future->cond);
	mutex_unlock(&future->mtx);
}

static int future_get(struct future *future)
{
	int res;

	mutex_lock(&future->mtx);
	while (!future->set)
		condition_wait(&future->cond, &future->mtx);
	res = future->value;
	mutex_unlock(&future->mtx);
	return res;
}

static void __th3_main(void *data)
{
	struct future *fut = data;

	wait(1000);
	future_set(fut, 42);
}

static void test_condition(void)
{
	struct future fut;
	struct thread *th = thread_create(&__th3_main, &fut);

	future_setup(&fut);
	thread_activate(th);
	BUG_ON(future_get(&fut) != 42);

	thread_join(th);
	thread_destroy(th);
}

void main(void *bootstrap_info)
{
	qemu_gdb_hang();

	serial_setup();
	ints_setup();
	time_setup();
        find_initrd(bootstrap_info);
	balloc_setup(bootstrap_info);
	paging_setup();
	page_alloc_setup();
	mem_alloc_setup();
	kmap_setup();
	threads_setup();
	enable_ints();
        initialize_ram_filesystem();
        initramfs();

	printf("Tests Begin\n");
	test_buddy();
	test_slab();
	test_alloc();
	test_kmap();
	test_threads();
	test_mutex();
	test_condition();
	printf("Tests Finished\n");

	idle();
}
