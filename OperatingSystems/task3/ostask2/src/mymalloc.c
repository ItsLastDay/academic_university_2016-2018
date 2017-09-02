#include <mymalloc.h>
#include <hashtable.h>
#include <buddy_alloc.h>
#include <slab_alloc.h>
#include <memory.h>
#include <printf.h>

/*
 * http://www.ezdoum.com/upload/20/20020412235505/bonwick94slab.pdf, page 4:
 *
 * "The standard non-caching allocation routines,
 * kmem_alloc(9F) and kmem_free(9F), use
 * object caches internally. At startup, the system
 * creates a set of about 30 caches ranging in size
 * from 8 bytes to 9K in roughly 10-20% increments.
 * kmem_alloc() simply performs a
 * kmem_cache_alloc() from the nearest-size
 * cache. Allocations larger than 9K, which are rare,
 * are handled directly by the back-end page supplier"
 */

#define BORDER_SIZE 9 * PAGE_SIZE + 1
#define MAX_SIZES 31

static struct slab_allocator* caches[MAX_SIZES];
static struct slab_allocator* allocs_from[BORDER_SIZE];
static struct hashtable* table = NULL;

static const int sizes[MAX_SIZES] = {8,
    10,
    12,
    15,
    19,
    24,
    30,
    38,
    48,
    60,
    76,
    96,
    121,
    153,
    194,
    246,
    312,
    396,
    502,
    637,
    808,
    1026,
    1303,
    1654,
    2100,
    2667,
    3387,
    4301,
    5462,
    6936,
    8808
};

void init_my_malloc()
{
    table = hashtable_create();

    for (int i = 0; i < MAX_SIZES; i++)
    {
        caches[i] = slab_allocator_create(sizes[i]);
    }

    for (size_t i = 0; i < BORDER_SIZE; i++)
        allocs_from[i] = NULL;

    for (size_t i = 0; i < BORDER_SIZE; i++)
    {
        for (int j = 0; j < MAX_SIZES; j++)
        {
            if (caches[j]->allocating_size >= i)
            {
                allocs_from[i] = caches[j];
                break;
            }
        }
    }
}

void* mymalloc(size_t size)
{
    if (size == 0)
        return NULL;

    void* addr;
    if (size < BORDER_SIZE && allocs_from[size] != NULL)
    {
        addr = slab_alloc_memory(allocs_from[size]);
        if (addr == SLAB_MEMORY_INVALID)
        {
            return NULL;
        }
    }
    else
    {
        size_t num_pages = size / PAGE_SIZE + (size % PAGE_SIZE != 0);
        addr = buddy_alloc_pages(num_pages);
        if (addr == BUDDY_ALLOC_FAIL)
        {
            return NULL;
        }
    }
    hashtable_put(table, (uintptr_t)addr, size);
    return addr;
}

void myfree(void* addr)
{
    size_t size = hashtable_query(table, (uintptr_t)addr);
    if (size == HASHTABLE_NOT_FOUND)
    {
        return;
    }

    if (size < BORDER_SIZE && allocs_from[size] != NULL)
    {
        slab_free_memory(allocs_from[size], addr);
    }
    else
    {
        size_t num_pages = size / PAGE_SIZE + (size % PAGE_SIZE != 0);
        buddy_dealloc_pages(addr, num_pages);
    }
}



/*
void test_malloc()
{
    void* addr = mymalloc(5);
    printf("%llx\n", addr);
    myfree(addr);
    addr = mymalloc(5);
    printf("%llx\n", addr);

    void* addr2 = mymalloc(12345);

    addr = mymalloc(12345);
    printf("%llx\n", addr);
    myfree(addr);
    addr = mymalloc(12345);
    printf("%llx\n", addr);

    myfree(addr2);
}*/
