#include <paging.h>
#include <stdint.h>
#include <memory.h>
#include <string.h>
#include <balloc.h>
#include <debug.h>
#include <alloc.h>


struct page_pool {
	uintptr_t (*get_page)(struct page_pool *);
	void (*put_page)(struct page_pool *, uintptr_t);
};


static uintptr_t pool_get_page(struct page_pool *pool)
{
	return pool->get_page(pool);
}

static void pool_put_page(struct page_pool *pool, uintptr_t page)
{
	pool->put_page(pool, page);
}

static int pml_shift(int level)
{
	static const int offs[] = {0, 12, 21, 30, 39};

	BUG_ON(level < 1 || level > 4);
	return offs[level];
}

static int pml_offs(uintptr_t addr, int level)
{
	static const int mask[] = {0xfff, 0x1ff, 0x1ff, 0x1ff, 0x1ff};

	BUG_ON(level > 4);
	return (addr >> pml_shift(level)) & mask[level];
}

static uintptr_t pml_size(int level)
{
	BUG_ON(level < 1 || level > 4);
	return (uintptr_t)1 << pml_shift(level);
}

static int __pt_count_pages(uintptr_t begin, uintptr_t end, uintptr_t phys,
			int lvl)
{
	const uintptr_t size = pml_size(lvl);
	const uintptr_t mask = size - 1;

	const int from = pml_offs(begin, lvl);
	const int to = pml_offs(end - 1, lvl) + 1;
	const int notaligned = (begin & mask) || (phys & mask);

	int count = 1;

	if (lvl == 1)
		return count;

	for (int i = from; i != to; ++i) {
		const uintptr_t entry_end = (begin + size) & ~mask;
		const uintptr_t map_begin = begin;
		const uintptr_t map_end = entry_end < end ? entry_end : end;
		const uintptr_t tomap = map_end - map_begin;

		if (lvl == 4 || notaligned)
			count += __pt_count_pages(map_begin, map_end, phys,
						lvl - 1);

		begin += tomap;
		phys += tomap;
	}

	return count;
}

static int pt_count_pages(uintptr_t begin, uintptr_t end, uintptr_t phys)
{
	return __pt_count_pages(begin, end, phys, 4);
}

/* Well, i have no much to say this function is a complete mess. */
static void __pt_map_pages(pte_t *pml, uintptr_t begin, uintptr_t end,
			uintptr_t phys, pte_t flags,
			int lvl, struct page_pool *pool)
{
	const pte_t pde_flags = __PTE_PRESENT | PTE_WRITE | PTE_USER;
	const uintptr_t size = pml_size(lvl);
	const uintptr_t mask = size - 1;

	const int from = pml_offs(begin, lvl);
	const int to = pml_offs(end, lvl);
	const pte_t pte_large = (lvl == 3 || lvl == 2)
				? (flags & __PTE_LARGE) : 0;

	for (int i = from; i <= to; ++i) {
		const int aligned = !((begin & mask) || (phys & mask));
		const uintptr_t entry_end = ((begin + size) & ~mask) - 1;
		const uintptr_t map_end = entry_end < end ? entry_end : end;

		const uintptr_t map_begin = begin;
		const uintptr_t tomap = map_end - map_begin + 1;

		pte_t pte = pml[i];

		if (!(pte & __PTE_PRESENT)) {
			const int map = (lvl != 4) && tomap == size && aligned
						&& (pte_large || lvl == 1);

			if (map) {
				pml[i] = (pte_t)phys | flags | pte_large;
				begin += tomap;
				phys += tomap;
				continue;
			}
			BUG_ON(lvl == 1);

			const uintptr_t page = pool_get_page(pool);

			/* I prefer to preallocate all required pages
			 * beforehand, so that mapping itself cannot fail,
			 * and thus this BUG_ON. */
			BUG_ON(!page);
			pte = pml[i] = (pte_t)page | pde_flags;
		}

		BUG_ON(lvl == 1);
		__pt_map_pages(va(pte & PTE_PHYS_MASK),
					map_begin, map_end,
					phys, flags, lvl - 1, pool);
		begin += tomap;
		phys += tomap;
	}
}

static void pt_map_pages(pte_t *pml4, uintptr_t begin, uintptr_t end,
			uintptr_t phys, pte_t flags, struct page_pool *pool)
{
	BUG_ON(begin & PAGE_MASK);
	BUG_ON(end & PAGE_MASK);

	/* We pass end - 1 to avoid overflows, and __pt_map_pages assumes
	 * both ends of the interval are included in the interval. */
	__pt_map_pages(pml4, begin, end - 1, phys,
				flags | __PTE_PRESENT | __PTE_LARGE, 4, pool);
}

static void pt_populate_pages(pte_t *pml4, uintptr_t begin, uintptr_t end,
			struct page_pool *pool)
{
	BUG_ON(begin & PAGE_MASK);
	BUG_ON(end & PAGE_MASK);

	__pt_map_pages(pml4, begin, end - 1, 0, 0, 4, pool);
}

static void write_cr3(uintptr_t addr)
{ __asm__ ("movq %0, %%cr3" : : "r"(addr)); }

static uintptr_t read_cr3(void)
{
	uintptr_t addr;

	__asm__ ("movq %%cr3, %0" : "=r"(addr));
	return addr;
}

static void flush_tlb_addr(const void *addr)
{ __asm__ volatile ("invlpg (%0)" : : "r"(addr) : "memory"); }

static uintptr_t paging_setup_get_page(struct page_pool *pool)
{
	(void) pool;

	/* Since we can't work without propper page table, when
	 * we setup our first page table we can allocate pages
	 * one by one and just fail if allocation failed. */
	const uintptr_t page = __balloc_alloc(
				/* size = */PAGE_SIZE,
				/* align =  */PAGE_SIZE,
				/* from = */0,
				/* to =  */BOOTSTRAP_MEM);
	pte_t *pte = va(page);

	BUG_ON(page == BOOTSTRAP_MEM &&
			"Not enough free memory to setup initial page table");
	memset(pte, 0, PAGE_SIZE);
	return page;
}

void paging_setup(void)
{
	const uintptr_t phys_mem_limit = balloc_memory() & ~PAGE_MASK;
	const uintptr_t mem_size = phys_mem_limit < MAX_PMEM_SIZE
				? phys_mem_limit : MAX_PMEM_SIZE;

	struct page_pool pool = { &paging_setup_get_page, 0 };
	const uintptr_t pml4 = pool_get_page(&pool);
	pte_t *pte = va(pml4);

	pt_map_pages(pte, HIGHER_BASE, HIGHER_BASE + mem_size, 0,
				PTE_WRITE, &pool);

	pt_map_pages(pte, VIRTUAL_BASE, 0, 0, PTE_WRITE, &pool);
	write_cr3(pml4);

	/* Not used yet, so we need to shut up compiler. */
	(void) pool_put_page;
	(void) pt_count_pages;
}


#define KMAP_ORDERS 16
#define KMAP_BEGIN  (VIRTUAL_BASE - KMAP_SIZE)
#define KMAP_END    VIRTUAL_BASE
#define KMAP_PAGES  (KMAP_SIZE >> PAGE_SHIFT)

struct kmap_range {
	struct list_head ll;
	size_t pages;
};

static struct kmap_range *kmap_ranges;
struct list_head kmap_free_ranges[KMAP_ORDERS];

static struct kmap_range *addr_kmap(const void *addr)
{
	const size_t no = ((uintptr_t)addr - KMAP_BEGIN) >> PAGE_SHIFT;

	return &kmap_ranges[no];
}

static void *kmap_addr(const struct kmap_range *range)
{
	const size_t no = range - kmap_ranges;

	return (void *)(KMAP_BEGIN + (no << PAGE_SHIFT));
}

static int kmap_order(size_t pages)
{
	size_t order = 0;

	while (((size_t)1 << order) < pages && order != KMAP_ORDERS)
		++order;
	return order;
}

static struct kmap_range *kmap_find_free(int order, size_t pages)
{
	struct list_head *head = &kmap_free_ranges[order];
	struct list_head *ptr = head->next;

	for (; ptr != head; ptr = ptr->next) {
		struct kmap_range *range = LIST_ENTRY(ptr,
					struct kmap_range, ll);

		if (range->pages >= pages)
			return range;
	}
	return 0;
}

static void kmap_free_range(struct kmap_range *range, size_t pages)
{
	if (range > kmap_ranges) {
		struct kmap_range *prev = range - (range - 1)->pages;

		if (!list_empty(&prev->ll)) {
			list_del(&prev->ll);
			pages += prev->pages;
			range = prev;
		}
	}

	if (range + pages < kmap_ranges + KMAP_PAGES) {
		struct kmap_range *next = range + pages;

		if (!list_empty(&next->ll)) {
			list_del(&next->ll);
			pages += next->pages;
		}
	}

	(range + pages - 1)->pages = range->pages = pages;
	list_add(&range->ll, &kmap_free_ranges[kmap_order(pages + 1) - 1]);
}

static struct kmap_range *kmap_alloc_range(size_t pages)
{
	const int start = kmap_order(pages);
	struct kmap_range *range = 0;

	for (int order = start; order != KMAP_ORDERS; ++order) {
		range = kmap_find_free(order, pages);

		if (range)
			break;
	}

	if (!range && ((size_t)1 << start) > pages)
		range = kmap_find_free(start - 1, pages);

	if (!range)
		return 0;

	const size_t range_pages = range->pages;

	list_del(&range->ll);
	list_init(&range->ll);
	(range + pages - 1)->pages = range->pages = pages;

	if (range_pages > pages)
		kmap_free_range(range + pages, range_pages - pages);
	return range;
}


static void *__kmap(struct page **pages, size_t count)
{
	if (!count)
		return 0;

	if (count == 1)
		return va(page_addr(pages[0]));

	struct kmap_range *range = kmap_alloc_range(count);

	if (!range)
		return 0;

	const uintptr_t pml4 = read_cr3();
	pte_t *pte = va(pml4);

	const uintptr_t from = (uintptr_t)kmap_addr(range);
	const uintptr_t to = from + count * PAGE_SIZE;
	int i = 0;

	for (uintptr_t addr = from; addr != to; addr += PAGE_SIZE) {
		pt_map_pages(pte, addr, addr + PAGE_SIZE,
					page_addr(pages[i++]), PTE_WRITE, 0);
		flush_tlb_addr((const void *)addr);
	}

	return kmap_addr(range);
}

static void __kunmap(void *ptr)
{
	if ((uintptr_t)ptr < KMAP_BEGIN || (uintptr_t)ptr >= KMAP_END)
		return;

	const uintptr_t pml4 = read_cr3();

	struct kmap_range *range = addr_kmap(ptr);
	const size_t count = range->pages;
	const uintptr_t from = (uintptr_t)ptr;
	const uintptr_t to = from + count * PAGE_SIZE;

	for (uintptr_t addr = from; addr != to; addr += PAGE_SIZE) {
		const int index[] = {
			0, pml_offs(addr, 1), pml_offs(addr, 2),
			pml_offs(addr, 3), pml_offs(addr, 4)
		};
		/* Just a little trick to avoid special cases and so on. */
		pte_t __pte = pml4;
		pte_t *pte = &__pte;

		for (int l = 4; l > 0; --l) {
			const uintptr_t next = *pte & PTE_PHYS_MASK;

			pte = (pte_t *)va(next) + index[l];
		}
		*pte = 0;	
		flush_tlb_addr((const void *)addr);
	}

	kmap_free_range(range, range->pages);
}

static uintptr_t kmap_setup_get_page(struct page_pool *pool)
{
	(void) pool;

	const uintptr_t addr = page_alloc(0);
	pte_t *pte = va(addr);

	BUG_ON(!addr);
	memset(pte, 0, PAGE_SIZE);
	return addr;	
}


static struct spinlock kmap_lock;

void *kmap(struct page **pages, size_t count)
{
	const int enable = spin_lock_irqsave(&kmap_lock);
	void *ptr = __kmap(pages, count);

	spin_unlock_irqrestore(&kmap_lock, enable);
	return ptr;
}

void kunmap(void *ptr)
{
	const int enable = spin_lock_irqsave(&kmap_lock);

	__kunmap(ptr);
	spin_unlock_irqrestore(&kmap_lock, enable);
}

void kmap_setup(void)
{
	static const size_t size = sizeof(struct kmap_range);

	struct page_pool pool = { &kmap_setup_get_page, 0 };
	uintptr_t pml4 = read_cr3();
	pte_t *pte = va(pml4);

	pt_populate_pages(pte, KMAP_BEGIN, KMAP_END, &pool);
	kmap_ranges = mem_alloc(size * KMAP_PAGES);
	BUG_ON(!kmap_ranges);

	spin_setup(&kmap_lock);

	for (int i = 0; i != KMAP_ORDERS; ++i)
		list_init(&kmap_free_ranges[i]);
	kmap_free_range(kmap_ranges, KMAP_PAGES);
}
