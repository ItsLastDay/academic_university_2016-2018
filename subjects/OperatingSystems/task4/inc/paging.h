#ifndef __PAGING_H__
#define __PAGING_H__

#include <stdint.h>
#include <rbtree.h>

#define __PTE_PRESENT	((pte_t)1 << 0)
#define __PTE_LARGE	((pte_t)1 << 7)

#define PTE_WRITE	((pte_t)1 << 1)
#define PTE_USER	((pte_t)1 << 2)

#define PTE_PHYS_MASK	((pte_t)0xffffffffff000)
#define PTE_FLAGS_MASK	(~PTE_PHYS_MASK)

#define PT_ENTRIES	512


typedef uint64_t pte_t;

void paging_setup(void);


struct page;

void *kmap(struct page **pages, size_t count);
void kunmap(void *ptr);
void kmap_setup(void);

#endif /*__PAGING_H__*/
