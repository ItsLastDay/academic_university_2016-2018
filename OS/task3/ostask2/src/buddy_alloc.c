#include <buddy_alloc.h>
#include <printf.h>
#include <memory.h>
#include <stdint.h>

// 1 bit for every block of every order: is it free or not.
static uint64_t is_free_block[MAX_NUM_BLOCKS / 64 + 1];

static void dump_buddy_structure();

// Throughout the code, `block_num` is a number from 0 to (1 << real_max_order) - 1.
static int real_max_order;
// Logical start address.
static uintptr_t buddy_buffer_start;

struct BuddyDescriptor head_list;

static void test_buddy_alloc();

static uint64_t get_pos(uint64_t block_num, int order)
{
    /*
     * Layout of `is_free_block` array:
     *
     * <all blocks of order 0>, <all blocks of order 1>, ..., <block of order `real_max_order`>.
     *
     * We know the number of blocks of each type: it is 2 ** (real_max_order - i).
     */
    uint64_t offset = 0;
    for (int i = 0; i < order; i++)
    {
        offset += (1ll << (real_max_order - i));
        block_num >>= 1;
    }

    return offset + block_num;
}

static uintptr_t get_block_start_mem(uint64_t block_num)
{
    return buddy_buffer_start + block_num * PAGE_SIZE;
}

static uint64_t get_blocknum_by_addr(uintptr_t addr)
{
    return (addr - buddy_buffer_start) / PAGE_SIZE;
}

static int get_ith_bit(uint64_t bit_no)
{
    uint64_t cell = bit_no / 64;
    bit_no -= cell * 64;
    return (is_free_block[cell] >> bit_no) & 1;
}

static void set_ith_bit(uint64_t bit_no, int val)
{
    uint64_t cell = bit_no / 64;
    bit_no -= cell * 64;

    is_free_block[cell] ^= ((is_free_block[cell] >> bit_no) & 1) << bit_no;
    is_free_block[cell] |= (1ull << bit_no) * val;
}

static uintptr_t get_aligned_down_addr(uintptr_t addr)
{
    addr &= ~(PAGE_SIZE - 1);
    return addr;
}

static void set_block_free(uint64_t block_num, int order)
{
    set_ith_bit(get_pos(block_num, order), 1);
}

static void set_block_used(uint64_t block_num, int order)
{
    set_ith_bit(get_pos(block_num, order), 0);
}

static int is_block_free(uint64_t block_num, int order)
{
    return get_ith_bit(get_pos(block_num, order)) == 1;
}

static void add_to_free_list(uint64_t block_num, int order)
{
    struct BuddyDescriptor* desc = (struct BuddyDescriptor*)
        get_block_start_mem(block_num);
   
    desc->nodes[order].prev = &(head_list.nodes[order]); 
    desc->nodes[order].next = head_list.nodes[order].next;
    if (desc->nodes[order].next != NULL)
    {
        desc->nodes[order].next->prev = &(desc->nodes[order]);
    }
    head_list.nodes[order].next = &(desc->nodes[order]);
}

static void erase_from_free_list(struct BuddyListNode* elem)
{
    // `prev` is always non-NULL.
    struct BuddyListNode* prev = elem->prev;
    struct BuddyListNode* next = elem->next;
    prev->next = next;
    if (next != NULL)
    {
        next->prev = prev;
    }
}

static int is_order_list_empty(int order)
{
    return head_list.nodes[order].next == NULL;
}

static int get_higher_power_of_two(uint64_t num)
{
    int order = 0;
    while ((1ull << order) < num)
    {
        order++;
    }
    return order;
}

static void free_block(uint64_t block_num, int order)
{
    set_block_free(block_num, order);
    add_to_free_list(block_num, order);
}

static void use_block(uint64_t block_num, int order)
{
    set_block_used(block_num, order);
    struct BuddyDescriptor* desc = (struct BuddyDescriptor*)
        get_block_start_mem(block_num);
    erase_from_free_list(&(desc->nodes[order]));
}

static size_t free_address_block(size_t region_start, size_t effective_length)
{
    if (effective_length == 0)
    {
        return 0;
    }

    for (int i = MAX_ORDER; i >= 0; i--)
    {
        size_t cur_size = 1ull << i;
        if (cur_size > effective_length)
        {
            continue;
        }
        // Try to align the region, so that it starts aligned by `cur_size`.
        size_t aligned_start = region_start;
        if (aligned_start & (cur_size - 1))
        {
            aligned_start &= ~(cur_size - 1);
            aligned_start += cur_size;
        }
        if (aligned_start - region_start + cur_size > effective_length)
        {
            continue;
        }
        // Buddies CANNOT be made using this algorithm. If a buddy exists,
        // then i + 1 should have been the last iteration of this loop.
        free_block(aligned_start, i);

        size_t num_pages = cur_size;
        num_pages += free_address_block(region_start, aligned_start - region_start);
        num_pages += free_address_block(aligned_start + cur_size,
                effective_length - (aligned_start + cur_size - region_start));
        return num_pages;
    }
    
    // Should never happen, because i=0 is always sufficient.
    return 0;
}

void init_buddy_allocator(multiboot_info_t* loader_info)
{
    multiboot_memory_map_t *mmap;
    size_t num_pages;

    // Nodes are stored inside blocks themselves, at the beginning.
    for (int i = 0; i <= real_max_order; i++)
    {
        head_list.nodes[i].prev = NULL;
        head_list.nodes[i].next = NULL;
    }

    // Always start from the physical addr=0. 
    // Mark all free memory as available to buddy allocator, with respect to orders.
    buddy_buffer_start = CANONICAL_HOLE_END;
    real_max_order = MAX_ORDER;
    size_t last_block_num = 1ull << MAX_ORDER;

    for (mmap = (multiboot_memory_map_t *) (uintptr_t)loader_info->mmap_addr;
        (unsigned long long) mmap < loader_info->mmap_addr + loader_info->mmap_length;
        mmap = (multiboot_memory_map_t *) ((unsigned long long) mmap
                                 + mmap->size + sizeof (mmap->size)))
    {
        if (mmap->type != MULTIBOOT_MEMORY_AVAILABLE)
        {
            continue;
        }

        uintptr_t region_start = mmap->addr;
        uintptr_t effective_length = mmap->len;
        if (region_start & (PAGE_SIZE - 1))
        {
            size_t remainder = region_start & (PAGE_SIZE - 1);
            effective_length -= PAGE_SIZE - remainder;
            region_start += PAGE_SIZE - remainder;
        }

        effective_length /= PAGE_SIZE;
        region_start /= PAGE_SIZE;

        // Limit the region at MAX_ORDER border.
        if (region_start >= last_block_num)
        {
            continue;
        }
        size_t cap_size = last_block_num - region_start;
        if (cap_size < effective_length)
        {
            effective_length = cap_size;
        }

        // Slice all the memory, producing valid blocks of some orders.
        // `valid` here means that block_num should be divisible by 2 ** order.
        // Blocks should be as large as possible.
        num_pages += free_address_block(region_start, effective_length);
    }

    printf("\nBuddy allocator successfully created!\n");
    printf("You can allocate up to %lld pages\n", num_pages);

    // Get rid of "unused function" warning.
    if (0)
        test_buddy_alloc();
}

static uint64_t get_needed_order(uint64_t num)
{
    return get_higher_power_of_two(num);
}

// Return valid logical address. Or 0, if allocation fails.
void* buddy_alloc_pages(uint64_t num_pages)
{
    int order = get_needed_order(num_pages);

    int free_node_order = -1;
    for (int i = order; i <= real_max_order; i++)
    {
        if (is_order_list_empty(i))
        {
            continue;
        }
        free_node_order = i;
        break;
    }

    if (free_node_order == -1)
    {
        return BUDDY_ALLOC_FAIL;
    }

    for (int j = free_node_order; j > order; j--)
    {
        // We can get the beginning of any block by a pointer to it's ListNode. 
        uintptr_t addr_block_begin = 
            get_aligned_down_addr((uint64_t)head_list.nodes[j].next);
        uint64_t block_num = get_blocknum_by_addr(addr_block_begin);

        use_block(block_num, j);

        free_block(block_num + (1ull << (j - 1)), j - 1);
        free_block(block_num, j - 1);
    }

    uintptr_t addr_block_begin = 
        get_aligned_down_addr((uint64_t)head_list.nodes[order].next);
    uint64_t block_num = get_blocknum_by_addr(addr_block_begin);
    use_block(block_num, order);

    return (void*)addr_block_begin;
}

// http://mit.spbau.ru/sewiki/images/c/c6/Lec3.pdf, page 16:
// "мы всегда будем алоцировать столько, сколько
// запросили и пользователь сам может хранить размер;".
//
// Therefore, user should specify the number of pages that were
// allocated.
void buddy_dealloc_pages(void* addr, uint64_t num_pages)
{
    int order = get_needed_order(num_pages);
    uint64_t block_num = get_blocknum_by_addr((uintptr_t)addr);
    free_block(block_num, order);

    while (order < real_max_order)
    {
        uint64_t buddy_num = ((block_num >> order) ^ 1) << order;
        if (is_block_free(buddy_num, order))
        {
            use_block(buddy_num, order);
            use_block(block_num, order);
            if (buddy_num < block_num)
            {
                block_num = buddy_num;
            }
            free_block(block_num, order + 1);
            order++;
        } 
        else
        {
            break;
        }
    }
}


void dump_buddy_structure()
{
    // For debug purposes.
    
    printf("/*********************************************/\n");
    for (int order = real_max_order; order >= 0; order--)
    {
        printf("Order %d:\n\t", order);
        struct BuddyListNode* elem = head_list.nodes[order].next;
        
        while (elem != NULL)
        {
            uint64_t block_num = 
                get_blocknum_by_addr(get_aligned_down_addr((uintptr_t)elem));
            printf("%lld ", block_num);
            if (!is_block_free(block_num, order))
            {
                printf("_failed\n");
                __asm__("int $88");
            }
            elem = elem->next;
        }

        printf("\n");
    }
    printf("/*********************************************/\n\n");
}

void test_buddy_alloc()
{
    uint8_t* addr;
    uint8_t* addr1;
    uint8_t* addr2;
    uint8_t* addr3;
    uint8_t* addr4;
    dump_buddy_structure();

    
    addr = buddy_alloc_pages(12);
   
    for(int i = 0; i < 1050; i++)
        *(addr + i) = 'Q';

    dump_buddy_structure();

    
    addr1 = buddy_alloc_pages(12345);
    printf("Block %lld is used\n", get_blocknum_by_addr((uintptr_t)addr1));
    addr2 = buddy_alloc_pages(12345);
    printf("Block %lld is used\n", get_blocknum_by_addr((uintptr_t)addr2));
    addr3 = buddy_alloc_pages(12345);
    printf("Block %lld is used\n", get_blocknum_by_addr((uintptr_t)addr3));
    addr4 = buddy_alloc_pages(12345);
    printf("Block %lld is used\n", get_blocknum_by_addr((uintptr_t)addr4));
    dump_buddy_structure();
    

    buddy_dealloc_pages(addr, 12);
    dump_buddy_structure();

    buddy_dealloc_pages(addr1, 12345);
    buddy_dealloc_pages(addr2, 12345);
    buddy_dealloc_pages(addr3, 12345);
    buddy_dealloc_pages(addr4, 12345);

    dump_buddy_structure();



    addr1 = buddy_alloc_pages(12345);
    printf("Block %lld is used\n", get_blocknum_by_addr((uintptr_t)addr1));
    addr2 = buddy_alloc_pages(12345);
    printf("Block %lld is used\n", get_blocknum_by_addr((uintptr_t)addr2));
    addr3 = buddy_alloc_pages(12345);
    printf("Block %lld is used\n", get_blocknum_by_addr((uintptr_t)addr3));
    addr4 = buddy_alloc_pages(12345);
    printf("Block %lld is used\n", get_blocknum_by_addr((uintptr_t)addr4));
    dump_buddy_structure();

    buddy_dealloc_pages(addr4, 12345);
    buddy_dealloc_pages(addr3, 12345);
    buddy_dealloc_pages(addr2, 12345);
    buddy_dealloc_pages(addr1, 12345);

    dump_buddy_structure();
}
