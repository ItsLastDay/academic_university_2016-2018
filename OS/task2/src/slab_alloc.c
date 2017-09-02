#include <slab_alloc.h>
#include <printf.h>
#include <stdint.h>
#include <stddef.h>
#include <buddy_alloc.h>
#include <memory.h>
#include <hashtable.h>
#include <spinlock.h>

#define SMALL_OBJECT_SIZE ((PAGE_SIZE) / 8)
#define PAGES_FOR_LARGE_SLAB 32


static spinlock_t slab_destroy_lock;

static const int small_slab_info_offset = PAGE_SIZE - sizeof(struct slab);

// Allocator of "struct slab_allocator" objects.
static spinlock_t sl_struct_lock;
struct slab_allocator sl_struct_allocator = {sizeof(struct slab_allocator), NULL, NULL, &sl_struct_lock};

static spinlock_t slab_buf_struct_lock;
struct slab_allocator slab_buf_struct_allocator = {sizeof(struct slab_buf), NULL, NULL, &slab_buf_struct_lock};

static spinlock_t slab_struct_lock;
struct slab_allocator slab_struct_allocator = {sizeof(struct slab), NULL, NULL, &slab_struct_lock};

static spinlock_t spinlock_lock;
struct slab_allocator spinlock_allocator = {sizeof(spinlock_t), NULL, NULL, &spinlock_lock};

static uintptr_t get_aligned_down_addr(uintptr_t addr)
{
    addr &= ~(PAGE_SIZE - 1);
    return addr;
}

static void dealloc_small_slab(struct slab* free_slab)
{
    uintptr_t base_slab = get_aligned_down_addr((uintptr_t)free_slab);
    buddy_dealloc_pages((void*)base_slab, 1);
}

static void dealloc_large_slab(struct slab* free_slab)
{
    uintptr_t min_buf_addr = -1;
    while (free_slab->free_list_begin != NULL)
    {
        struct slab_buf* next_free = free_slab->free_list_begin->next_free_buf;
        uintptr_t cur_addr = (uintptr_t)free_slab->free_list_begin->buf_addr;
        if (cur_addr < min_buf_addr)
        {
            min_buf_addr = cur_addr;
        }
        slab_free_memory(&slab_buf_struct_allocator, free_slab->free_list_begin);
        free_slab->free_list_begin = next_free;
    }
    buddy_dealloc_pages((void*)min_buf_addr, PAGES_FOR_LARGE_SLAB);
    slab_free_memory(&slab_struct_allocator, free_slab);
}

static void small_slab_add_free_buf(struct slab* slab_info, uintptr_t buf)
{
    // Store only offsets from the page start.
    uintptr_t slab_base = get_aligned_down_addr((uintptr_t)slab_info);
    uint16_t cur_head = (uint16_t)(uintptr_t)slab_info->free_list_begin;
    slab_info->free_list_begin = (struct slab_buf*)(buf - slab_base);
    *((uint16_t*)buf) = cur_head;
}

static void large_slab_add_free_buf(struct slab* slab_info, uintptr_t buf,
        struct slab_allocator* allocator)
{
    struct slab_buf* buf_info = slab_alloc_memory(&slab_buf_struct_allocator);
    buf_info->controlling_slab = slab_info;
    buf_info->buf_addr = (void*)buf;
    buf_info->next_free_buf = slab_info->free_list_begin;
    slab_info->free_list_begin = buf_info;
    hashtable_put(allocator->addr_to_buf, (uintptr_t)buf, (uintptr_t)buf_info);
}

static void init_small_slab(void* buf, struct slab_allocator* allocator)
{
    // Add small slab to previously empty allocator.
    uintptr_t buf_base = (uintptr_t)buf;
    struct slab* slab_info = (struct slab*)(buf_base + small_slab_info_offset);
    slab_info->refcount = 0;
    slab_info->next = NULL;
    slab_info->prev = NULL;
    slab_info->free_list_begin = (struct slab_buf*)PAGE_SIZE;

    for (int i = 0; buf_base + (i + 1) * allocator->allocating_size 
            <= (uintptr_t)slab_info; i++)
    {
        small_slab_add_free_buf(slab_info, buf_base + i * allocator->allocating_size);
    }

    allocator->free_slab_begin = slab_info;
}

static void init_large_slab(void* buf, struct slab_allocator* allocator)
{
    // Add large slab to previously empty allocator.
    uintptr_t buf_base = (uintptr_t)buf;
    struct slab* slab_info = slab_alloc_memory(&slab_struct_allocator);
    slab_info->refcount = 0;
    slab_info->next = NULL;
    slab_info->prev = NULL;
    uintptr_t buf_end = buf_base + PAGES_FOR_LARGE_SLAB * PAGE_SIZE;

    for (int i = 0; buf_base + (i + 1) * allocator->allocating_size <= buf_end; i++)
    {
        large_slab_add_free_buf(slab_info, buf_base + i * allocator->allocating_size,
                allocator);
    }

    allocator->free_slab_begin = slab_info;
}

static void* slab_alloc_small_memory(struct slab_allocator* allocator)
{
    if (allocator->free_slab_begin == NULL)
    {
        void* buf = buddy_alloc_pages(1);
        if (buf == 0)
        {
            return SLAB_MEMORY_INVALID;
        }
        init_small_slab(buf, allocator);
    }
    
    struct slab* free_slab = allocator->free_slab_begin;
    uintptr_t ret_addr = ((uintptr_t)free_slab->free_list_begin) + 
        get_aligned_down_addr((uintptr_t)free_slab);
    free_slab->refcount++;
    free_slab->free_list_begin = (struct slab_buf*)(uintptr_t)*((uint16_t*)ret_addr);
    if ((uintptr_t)free_slab->free_list_begin == PAGE_SIZE)
    {
        // Slab is full: delete it from allocator's free list.
        allocator->free_slab_begin = free_slab->next;
        allocator->free_slab_begin->prev = NULL;
    }

    return (void*)ret_addr;
}

static void* slab_alloc_large_memory(struct slab_allocator* allocator)
{
    if (allocator->free_slab_begin == NULL)
    {
        void* buf = buddy_alloc_pages(PAGES_FOR_LARGE_SLAB);
        if (buf == 0)
        {
            return SLAB_MEMORY_INVALID;
        }
        init_large_slab(buf, allocator);
    }

    struct slab* free_slab = allocator->free_slab_begin;
    struct slab_buf* free_buf = free_slab->free_list_begin;
    free_slab->free_list_begin = free_buf->next_free_buf;
    free_slab->refcount++;

    if (free_slab->free_list_begin == NULL)
    {
        allocator->free_slab_begin = free_slab->next;
        allocator->free_slab_begin->prev = NULL;
    }

    return free_buf->buf_addr;
}

static void slab_free_memory_unlocked(struct slab_allocator* allocator, void* addr)
{
    struct slab* slab_info;
    uint16_t was_head = 0;
    struct slab_buf* prev_head = (void*)1234;

    if (allocator->allocating_size <= SMALL_OBJECT_SIZE)
    {
        uintptr_t slab_base = get_aligned_down_addr((uintptr_t)addr);
        slab_info = (struct slab*)(slab_base + small_slab_info_offset);
        was_head = (uint16_t)(uintptr_t)slab_info->free_list_begin;
        slab_info->refcount--;
        small_slab_add_free_buf(slab_info, (uintptr_t)addr);
    }
    else
    {
        struct slab_buf* buf_info = 
            (struct slab_buf*)hashtable_query(allocator->addr_to_buf, (uintptr_t)addr);
        slab_info = buf_info->controlling_slab;
        prev_head = slab_info->free_list_begin;
        slab_info->free_list_begin = buf_info;
        buf_info->next_free_buf = prev_head;
        slab_info->refcount--;
    }

    // Slab was full, but now it has free places - add it to allocator's list.
    if (prev_head == NULL || was_head == PAGE_SIZE)
    {
        struct slab* cur_head = allocator->free_slab_begin;
        slab_info->next = cur_head; 
        slab_info->prev = NULL;
        if (cur_head != NULL)
        {
            cur_head->prev = slab_info;
        }
        allocator->free_slab_begin = slab_info;
    }

    // Slab is completely empty - destroy it.
    if (slab_info->refcount == 0)
    {
        if (allocator->free_slab_begin == slab_info) 
        {
            allocator->free_slab_begin = slab_info->next;
            if (allocator->free_slab_begin != NULL)
            {
                allocator->free_slab_begin->prev = NULL;
            }
        }
        else
        {
            struct slab* prev = slab_info->prev;
            struct slab* next = slab_info->next;
            prev->next = next;
            next->prev = prev;
        }

        if (allocator->allocating_size <= SMALL_OBJECT_SIZE)
        {
            dealloc_small_slab(slab_info);
        }
        else
        {
            dealloc_large_slab(slab_info);
        }
    }
}


void slab_free_memory(struct slab_allocator* allocator, void* addr)
{
    spin_lock(allocator->lock);
    slab_free_memory_unlocked(allocator, addr);
    spin_unlock(allocator->lock);
}


static struct slab_allocator* slab_allocator_create_unlocked(size_t size)
{
    if (size == 1)
    {
        size = 2;
    }
    struct slab_allocator* allocator = slab_alloc_memory(&sl_struct_allocator); 
    allocator->allocating_size = size;
    allocator->free_slab_begin = NULL;
    allocator->lock = slab_alloc_memory(&spinlock_allocator);
    spin_init(allocator->lock);
    if (size > SMALL_OBJECT_SIZE)
    {
        allocator->addr_to_buf = hashtable_create();
    }
    else
    {
        allocator->addr_to_buf = NULL;
    }
    return allocator;
}


struct slab_allocator* slab_allocator_create(size_t size)
{
    // No need to take lock here, because it will be taken inside.
    struct slab_allocator* result = slab_allocator_create_unlocked(size);
    return result;
}


static void slab_allocator_destroy_unlocked(struct slab_allocator* allocator)
{
    // Memory leaks if some slab, allocated by this allocator, is not in free list.
    // But we don't tolerate incorrect usage of allocator.
    struct slab* free_slab = allocator->free_slab_begin;
    while (free_slab != NULL)
    {
        struct slab* next_free = free_slab->next;
        if (allocator->allocating_size <= SMALL_OBJECT_SIZE)
        {
            dealloc_small_slab(free_slab);
        }
        else
        {
            dealloc_large_slab(free_slab);
        }
        free_slab = next_free;
    }
    hashtable_destroy(allocator->addr_to_buf);
    slab_free_memory(&spinlock_allocator, allocator->lock);
    slab_free_memory(&sl_struct_allocator, allocator);
}


void slab_allocator_destroy(struct slab_allocator* allocator)
{
    spin_lock(&slab_destroy_lock);
    slab_allocator_destroy_unlocked(allocator);
    spin_unlock(&slab_destroy_lock);
}


static void* slab_alloc_memory_unlocked(struct slab_allocator* allocator)
{
    if (allocator->allocating_size <= SMALL_OBJECT_SIZE)
    {
        return slab_alloc_small_memory(allocator);
    }
    else
    {
        return slab_alloc_large_memory(allocator);
    }
}


void* slab_alloc_memory(struct slab_allocator* allocator)
{
    spin_lock(allocator->lock);
    void* result = slab_alloc_memory_unlocked(allocator);
    spin_unlock(allocator->lock);
    return result;
}



void test_slab_alloc_small()
{
    printf("Slab allocator test\n");
    struct slab_allocator* allocator = slab_allocator_create(4);
    uint64_t* num = slab_alloc_memory(allocator);
    printf("addr allocated = %llx\n", num);
    slab_free_memory(allocator, num);

    num = slab_alloc_memory(allocator);
    printf("addr allocated second time = %llx\n", num);
    slab_allocator_destroy(allocator);


    struct slab_allocator* almost_large_allocator = slab_allocator_create(512);
    uintptr_t addr[100];
    for (int i = 0; i < 10; i++)
    {
        addr[i] = (uintptr_t)slab_alloc_memory(almost_large_allocator);
        printf("almost large mem block %llx\n", addr[i]);
    }
    for (int i = 9; i >= 0; i--)
    {
        slab_free_memory(almost_large_allocator, (void*)addr[i]);
    }
    printf("\n");
    for (int i = 0; i < 10; i++)
    {
        addr[i] = (uintptr_t)slab_alloc_memory(almost_large_allocator);
        printf("almost large mem block %llx\n", addr[i]);
    }
    for (int i = 9; i >= 0; i--)
    {
        slab_free_memory(almost_large_allocator, (void*)addr[i]);
    }
    printf("\n");

    for (int i = 0; i < 60; i++)
    {
        addr[i] = (uintptr_t)slab_alloc_memory(almost_large_allocator);
        if (i < 10)
        {
            printf("almost large mem block %llx\n", addr[i]);
        }
        if (i % 7 == 3)
        {

            printf("jump mem %llx\n", addr[i]);
        }
    }
    for (int i = 3; i < 60; i += 7)
    {
        slab_free_memory(almost_large_allocator, (void*)addr[i]);
    }
    for (int i = 3; i < 90; i += 7)
    {
        addr[i] = (uintptr_t)slab_alloc_memory(almost_large_allocator);
        printf("almost large mem block %llx\n", addr[i]);
    }
}

void test_slab_alloc_large()
{
    printf("Slab alloc large test\n");

    struct slab_allocator* large_alloc = slab_allocator_create(PAGE_SIZE - 1);
    const int iter_size = 45;

    void* addr[100];
    for (int i = 0; i < iter_size; i++)
    {
        addr[i] = slab_alloc_memory(large_alloc);
        printf("%d'th addr is %llx\n", i, addr[i]);
    }
    for (int i = iter_size - 1; i >= 0; i--)
    {
        slab_free_memory(large_alloc, addr[i]);
    }
    printf("\n");
    for (int i = 0; i < iter_size; i++)
    {
        addr[i] = slab_alloc_memory(large_alloc);
        printf("%d'th addr is %llx\n", i, addr[i]);
    }
}


void init_slab(void)
{
    init_hashtable();

    spin_init(&sl_struct_lock);
    spin_init(&slab_buf_struct_lock);
    spin_init(&slab_struct_lock);
    spin_init(&spinlock_lock);
    spin_init(&slab_destroy_lock);

    if (0)
    {
        test_slab_alloc_large();
        test_slab_alloc_small();
    }
}
