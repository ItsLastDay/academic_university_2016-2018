#include <memory_map.h>
#include <printf.h>
#include <string.h>
#include <stdint.h>
#include <memory.h>
#include <buddy_alloc.h>
#include <mymalloc.h>
#include <slab_alloc.h>

static void print_memory_map();

static void add_kernel_area_to_memory_map(multiboot_info_t* loader_info);

static void extract_area_from_memory_map(multiboot_info_t* loader_info, 
        uintptr_t low, uintptr_t high);
static void remap_virtual_addresses();

static void* trivial_allocator(multiboot_info_t* loader_info, size_t size, size_t align);

static inline uintptr_t get_cr3()
{
	uintptr_t value;

	__asm__ volatile("movq %%cr3, %0" : "=r"(value));
	return value;
}

static inline void write_cr3(uint64_t val)
{
    __asm__ volatile ("movq %0,%%cr3" :: "r" (val) : "memory");
}

static void remap_virtual_addresses(multiboot_info_t* loader_info)
{
    struct PML4Entry* pml4_dir = (struct PML4Entry*)trivial_allocator(loader_info,
            PAGE_SIZE, PAGE_SIZE);
    struct PDPTEntry* pdpte_dir = (struct PDPTEntry*)trivial_allocator(loader_info,
            PAGE_SIZE, PAGE_SIZE);

    struct PML4Entry* last_pml4 = pml4_dir + 511;
    last_pml4->pdpt_p = (uintptr_t)pdpte_dir >> 12;
    last_pml4->present = 1;
    last_pml4->rw = 1;
    last_pml4 = pml4_dir + 256;
    last_pml4->pdpt_p = (uintptr_t)pdpte_dir >> 12;
    last_pml4->present = 1;
    last_pml4->rw = 1;
    last_pml4 = pml4_dir + 0;
    last_pml4->pdpt_p = (uintptr_t)pdpte_dir >> 12;
    last_pml4->present = 1;
    last_pml4->rw = 1;

    for (int i = 0; i < 512; i++)
    {
        struct PDPTEntry* cur_entry = pdpte_dir + i;

        cur_entry->present = 1;
        cur_entry->rw = 1;
        // We are inside kernel, users will have different page table.
        cur_entry->user = 0; 
        cur_entry->page_size = 1;

        cur_entry->phys_addr = i;

        cur_entry->reserved_1 = 0;
        cur_entry->xd = 0;
    }

    // Need to map the highest 2Gb to the lowest physical 2Gb.
    (pdpte_dir + 510)->phys_addr = 0;
    (pdpte_dir + 511)->phys_addr = 1;

    write_cr3((uintptr_t)pml4_dir);

    // Since we map only 510Gb of memory, extract the rest from memory map:
    extract_area_from_memory_map(loader_info, 547608330240, -1);
}

static void* trivial_allocator(multiboot_info_t* loader_info, size_t size,
        size_t align)
{
    uintptr_t mmap_end = (uintptr_t)(loader_info->mmap_addr + loader_info->mmap_length);
    multiboot_memory_map_t* mmap = (multiboot_memory_map_t*)(uintptr_t)loader_info->mmap_addr;
    size_t mmap_entry_size = mmap->size + sizeof(mmap->size);

    for (mmap = (multiboot_memory_map_t *) (uintptr_t)loader_info->mmap_addr;
        (uintptr_t)mmap < mmap_end;
        mmap = (multiboot_memory_map_t *) ((unsigned long long) mmap
                                 + mmap_entry_size))
    {
        uintptr_t cur_low = mmap->addr;

        if (mmap->type == MULTIBOOT_MEMORY_RESERVED)
        {
            continue;
        }

        size_t effective_len = mmap->len;
        if (cur_low % align != 0)
        {
            size_t mod = cur_low % align;
            cur_low += align - mod;
            effective_len -= align - mod;
        }

        if (effective_len < size)
        {
            continue;
        }

        extract_area_from_memory_map(loader_info, cur_low, cur_low + size);
        return (void*)cur_low;
    }

    // Should never happen if we have reasonable amount of memory.
    return (void*)((uintptr_t)CANONICAL_HOLE_END - 1);
}

static void copy_mmap_area(multiboot_info_t* loader_info, uintptr_t* res_low,
        uintptr_t* res_high)
{
    uintptr_t mmap_end = (uintptr_t)(loader_info->mmap_addr + loader_info->mmap_length);
    multiboot_memory_map_t* mmap = (multiboot_memory_map_t*)(uintptr_t)loader_info->mmap_addr;
    size_t mmap_entry_size = mmap->size + sizeof(mmap->size);

    // Other allocs in this file will produce small amount of additional nodes.
    size_t need_size = loader_info->mmap_length + 2 * PAGE_SIZE;

    for (mmap = (multiboot_memory_map_t *) (uintptr_t)loader_info->mmap_addr;
        (uintptr_t)mmap < mmap_end;
        mmap = (multiboot_memory_map_t *) ((unsigned long long) mmap
                                 + mmap_entry_size))
    {
        uintptr_t cur_low = mmap->addr;

        if (mmap->type == MULTIBOOT_MEMORY_RESERVED)
        {
            continue;
        }

        if (mmap->len < need_size)
        {
            continue;
        }

        *res_low = cur_low;
        *res_high = cur_low + need_size;
        memcpy((void*)*res_low, (void*)(uintptr_t)loader_info->mmap_addr, loader_info->mmap_length);
        loader_info->mmap_addr = cur_low;
        return;
    }
}

static void extract_area_from_memory_map(multiboot_info_t* loader_info, 
        uintptr_t low, uintptr_t high)
{
    uintptr_t mmap_end = (uintptr_t)(loader_info->mmap_addr + loader_info->mmap_length);
    multiboot_memory_map_t* mmap = (multiboot_memory_map_t*)(uintptr_t)loader_info->mmap_addr;
    size_t mmap_entry_size = mmap->size + sizeof(mmap->size);
    size_t mmap_size_value = mmap->size;

    for (mmap = (multiboot_memory_map_t *) (uintptr_t)loader_info->mmap_addr;
        (uintptr_t)mmap < mmap_end;
        mmap = (multiboot_memory_map_t *) ((unsigned long long) mmap
                                 + mmap_entry_size))
    {
        uintptr_t cur_low = mmap->addr;
        uintptr_t cur_high = cur_low + mmap->len;

        if (low >= cur_low && high <= cur_high)
        {
            if (mmap->type == MULTIBOOT_MEMORY_RESERVED)
            {
                // Memory is already reserved.
                break;
            }
            // Add two entries.
            memcpy(((char*)mmap) + 2 * mmap_entry_size, mmap, 
                    (char*)mmap_end - (char*)mmap);
            mmap->size = mmap_size_value;
            mmap->addr = cur_low;
            mmap->len = low - cur_low;
            mmap->type = MULTIBOOT_MEMORY_AVAILABLE;


            mmap = (multiboot_memory_map_t *) ((unsigned long long) mmap
                                     + mmap_entry_size);
            mmap->size = mmap_size_value;
            mmap->addr = low;
            mmap->len = high - low;
            mmap->type = MULTIBOOT_MEMORY_RESERVED;

            mmap = (multiboot_memory_map_t *) ((unsigned long long) mmap
                                     + mmap_entry_size);
            mmap->addr = high;
            mmap->len = cur_high - high;
            loader_info->mmap_length += 2 * mmap_entry_size;
            break;
        }

        if (low >= cur_low && low < cur_high && high > cur_high)
        {
            // The [low, high) segment covers some suffix of our region.
            // Just decrease `len` appropriately.
            mmap->len = low - cur_low;
        }
    }

}

static void add_kernel_area_to_memory_map(multiboot_info_t* loader_info)
{
    extern char text_phys_begin[];
    extern char bss_phys_end[];


    uintptr_t low = (uintptr_t)text_phys_begin;
    uintptr_t high = (uintptr_t)bss_phys_end;

    extract_area_from_memory_map(loader_info, low, high);
}


void init_memory(uintptr_t multiboot_loader_info)
{
    multiboot_info_t* loader_info = (multiboot_info_t*) multiboot_loader_info;
    
    uintptr_t mmap_area_low, mmap_area_high;
    copy_mmap_area(loader_info, &mmap_area_low, &mmap_area_high);
    extract_area_from_memory_map(loader_info, mmap_area_low, mmap_area_high);
    add_kernel_area_to_memory_map(loader_info);

    remap_virtual_addresses(loader_info);

    print_memory_map(loader_info);

    init_buddy_allocator(loader_info);
    init_slab();

    init_my_malloc();
}


static void print_memory_map(multiboot_info_t* loader_info)
{
    printf("\nMemory map:\n");
    if (loader_info->flags & MMAP_VALIDITY_BIT)
    {
        multiboot_memory_map_t *mmap;

        for (mmap = (multiboot_memory_map_t *) (uintptr_t)loader_info->mmap_addr;
            (unsigned long long) mmap < loader_info->mmap_addr + loader_info->mmap_length;
            mmap = (multiboot_memory_map_t *) ((unsigned long long) mmap
                                     + mmap->size + sizeof (mmap->size)))
        {
         printf (" base_addr = 0x%llx,"
                 " length = 0x%llx, type = 0x%x\n",
                 mmap->addr,
                 mmap->len,
                 (unsigned) mmap->type);
        }
    }   
}
