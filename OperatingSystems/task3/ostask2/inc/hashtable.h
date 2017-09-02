#ifndef __HASHTABLE_H__
#define __HASHTABLE_H__

#include <stddef.h>
#include <stdint.h>

#define HASHTABLE_NOT_FOUND 0

struct ht_kvpair
{
    uintptr_t key;
    uintptr_t val;
    struct ht_kvpair* next;
};

struct hashtable
{
    struct ht_kvpair** bucket_array;
    size_t number_of_entries;
    size_t current_size; // Always a power of two.
} __attribute__((packed));

void hashtable_destroy(struct hashtable*);
struct hashtable* hashtable_create();
uintptr_t hashtable_query(struct hashtable*, uintptr_t key);
void hashtable_put(struct hashtable*, uintptr_t key, uintptr_t value);

#endif
