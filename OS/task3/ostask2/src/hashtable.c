#include <hashtable.h>
#include <stdint.h>
#include <stddef.h>
#include <slab_alloc.h>
#include <buddy_alloc.h>

#define MAX_LOAD_FACTOR 1.2

const size_t shift = 0x234123054f;

struct slab_allocator ht_kvpair_struct_allocator = 
    {sizeof(struct ht_kvpair), NULL, NULL};
struct slab_allocator hashtable_struct_allocator = 
    {sizeof(struct hashtable), NULL, NULL};

static size_t get_hash(struct hashtable* table, uintptr_t key)
{
    // Just some random function.
    return ((key * 5) ^ shift) % table->current_size;
}

void hashtable_destroy(struct hashtable* table)
{
    if (table == NULL)
    {
        return;
    }

    for (size_t i = 0; i < table->current_size; i++)
    {
        struct ht_kvpair* head_list = table->bucket_array[i];
        while (head_list != NULL)
        {
            struct ht_kvpair* next = head_list->next;
            slab_free_memory(&ht_kvpair_struct_allocator, head_list);
            head_list = next;
        }
    }
    buddy_dealloc_pages(table->bucket_array, table->current_size);
    slab_free_memory(&hashtable_struct_allocator, table);
}

struct hashtable* hashtable_create()
{
    struct hashtable* table = slab_alloc_memory(&hashtable_struct_allocator);
    table->number_of_entries = 0;
    table->current_size = 1;
    // Expect buddy allocator to succeed.
    table->bucket_array = buddy_alloc_pages(table->current_size);
    table->bucket_array[0] = NULL;
    return table;
}

uintptr_t hashtable_query(struct hashtable* table, uintptr_t key)
{
    size_t bucket = get_hash(table, key);
    struct ht_kvpair* head_list = table->bucket_array[bucket]; 
    while (head_list != NULL)
    {
        if (head_list->key == key)
        {
            return head_list->val;
        }
        head_list = head_list->next;
    }

    return HASHTABLE_NOT_FOUND;
}

void hashtable_put(struct hashtable* table, uintptr_t key, uintptr_t value)
{
    size_t bucket = get_hash(table, key);
    struct ht_kvpair* head_list = table->bucket_array[bucket];

    // Maintain distinct keys.
    struct ht_kvpair* cur_head = head_list;

    while (cur_head != NULL)
    {
        if (cur_head->key == key)
        {
            cur_head->val = value;
            return;
        }
        cur_head = cur_head->next;
    }


    table->number_of_entries++;
    struct ht_kvpair* cur = slab_alloc_memory(&ht_kvpair_struct_allocator);
    cur->next = head_list;
    cur->key = key;
    cur->val = value;
    table->bucket_array[bucket] = cur;

    if (table->number_of_entries > MAX_LOAD_FACTOR * table->current_size)
    {
        struct ht_kvpair** new_buf = buddy_alloc_pages(table->current_size * 2);
        // If buddy is empty - leave the table as is.
        if (new_buf == 0)
        {
            return;
        }

        // Else, copy everything into new table, with rehashing.
        table->current_size *= 2;
        for (size_t i = 0; i < table->current_size; i++)
        {
            new_buf[i] = NULL;
        }

        for (size_t i = 0; i < table->current_size / 2; i++)
        {
            head_list = table->bucket_array[i];
            while (head_list != NULL)
            {
                struct ht_kvpair* next = head_list->next;
                
                size_t new_bucket = get_hash(table, head_list->key);
                cur = new_buf[new_bucket];
                head_list->next = cur;
                new_buf[new_bucket] = head_list;

                head_list = next;
            }
        }

        buddy_dealloc_pages(table->bucket_array, table->current_size / 2);

        table->bucket_array = new_buf;
    }
}




/*
void test_hashtable()
{
    struct hashtable* table = hashtable_create();

    for (int i = 10; i < 20; i++)
    {
        hashtable_put(table, i, 200 - i);
    }
    for (int i = 10; i < 20; i++)
    {
        hashtable_put(table, i, 100 - i);
    }
    for (int i = 10; i < 1200; i++)
    {
        hashtable_put(table, i, 10000 - i);
    }

    for (int i = 10; i < 1200; i++)
    {
        printf("%lld -> %lld\n", i, hashtable_query(table, i));
        if (i + hashtable_query(table, i) != 10000)
        {
            __asm__("int $88");
        }
    }
    printf("size = %lld, capacity = %lld\n", table->number_of_entries, table->current_size);
}
*/
