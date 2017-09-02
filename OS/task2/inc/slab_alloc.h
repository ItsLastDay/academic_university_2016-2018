#ifndef __SLAB_ALLOC_H__
#define __SLAB_ALLOC_H__

#include <spinlock.h>
#include <stdint.h>
#include <stddef.h>
#include <memory.h>
#include <hashtable.h>

#define SLAB_MEMORY_INVALID 0

struct slab;

struct slab_buf
{
    void* buf_addr;
    struct slab_buf* next_free_buf;
    struct slab* controlling_slab;
} __attribute__((packed));

struct slab
{
    size_t refcount;
    struct slab* prev;
    struct slab* next;
    struct slab_buf* free_list_begin;
} __attribute__((packed));

struct slab_allocator
{
    size_t allocating_size;
    struct slab* free_slab_begin;    
    struct hashtable* addr_to_buf;
    spinlock_t* lock;
} __attribute__((packed));

struct slab_allocator* slab_allocator_create(size_t);
void* slab_alloc_memory(struct slab_allocator*);
void slab_free_memory(struct slab_allocator*, void*);
void slab_allocator_destroy(struct slab_allocator*);
void init_slab(void);

spinlock_t* alloc_initialized_spinlock(void);

#endif
