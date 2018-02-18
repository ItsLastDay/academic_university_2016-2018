#ifndef __BUDDY_ALLOC_H__
#define __BUDDY_ALLOC_H__

#include <memory_map.h>

// Can manage up to 128Gb of memory.
#define MAX_ORDER 25
#define MAX_NUM_BLOCKS (1ll << (MAX_ORDER + 1))
#define BUDDY_ALLOC_FAIL 0

void init_buddy_allocator(multiboot_info_t*);

void* buddy_alloc_pages(uint64_t num_pages);
void buddy_dealloc_pages(void* addr, uint64_t num_pages);

struct BuddyListNode
{
    struct BuddyListNode* prev;
    struct BuddyListNode* next;
} __attribute__((packed));

struct BuddyDescriptor
{
    struct BuddyListNode nodes[MAX_ORDER + 1];
} __attribute__((packed));


#endif
