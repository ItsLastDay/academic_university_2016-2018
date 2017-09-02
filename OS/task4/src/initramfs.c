#include <initramfs.h>
#include <debug.h>
#include <memory.h>
#include <stdlib.h>
#include <balloc.h>
#include <string.h>
#include <print.h>
#include <ramfs.h>

// https://www.gnu.org/software/grub/manual/multiboot/multiboot.html#Boot-information-format
typedef uint16_t multiboot_uint16_t;
typedef uint32_t multiboot_uint32_t;
typedef uint64_t multiboot_uint64_t;

/* The section header table for ELF. */
struct multiboot_elf_section_header_table
{
    multiboot_uint32_t num;
    multiboot_uint32_t size;
    multiboot_uint32_t addr;
    multiboot_uint32_t shndx;
};
typedef struct multiboot_elf_section_header_table multiboot_elf_section_header_table_t;

/* The symbol table for a.out. */
struct multiboot_aout_symbol_table
{
    multiboot_uint32_t tabsize;
    multiboot_uint32_t strsize;
    multiboot_uint32_t addr;
    multiboot_uint32_t reserved;
};
typedef struct multiboot_aout_symbol_table multiboot_aout_symbol_table_t;

struct mboot_info {
   /* Multiboot info version number */
   multiboot_uint32_t flags;
 
   /* Available memory from BIOS */
   multiboot_uint32_t mem_lower;
   multiboot_uint32_t mem_upper;
 
   /* "root" partition */
   multiboot_uint32_t boot_device;
 
   /* Kernel command line */
   multiboot_uint32_t cmdline;
 
   /* Boot-Module list */
   multiboot_uint32_t mods_count;
   multiboot_uint32_t mods_addr;
 
   union
   {
     multiboot_aout_symbol_table_t aout_sym;
     multiboot_elf_section_header_table_t elf_sec;
   } u;
 
   /* Memory Mapping buffer */
   multiboot_uint32_t mmap_length;
   multiboot_uint32_t mmap_addr;
 
   /* Drive Info buffer */
   multiboot_uint32_t drives_length;
   multiboot_uint32_t drives_addr;
 
   /* ROM configuration table */
   multiboot_uint32_t config_table;
 
   /* Boot Loader Name */
   multiboot_uint32_t boot_loader_name;
 
   /* APM table */
   multiboot_uint32_t apm_table;
 
   /* Video */
   multiboot_uint32_t vbe_control_info;
   multiboot_uint32_t vbe_mode_info;
   multiboot_uint16_t vbe_mode;
   multiboot_uint16_t vbe_interface_seg;
   multiboot_uint16_t vbe_interface_off;
   multiboot_uint16_t vbe_interface_len;
} __attribute__((packed));

struct multiboot_mod_list
{
    /* the memory used goes from bytes 'mod_start' to 'mod_end-1' inclusive */
    multiboot_uint32_t mod_start;
    multiboot_uint32_t mod_end;

    /* Module command line */
    multiboot_uint32_t cmdline;

    /* padding to take it to 16 bytes (must be zero) */
    multiboot_uint32_t pad;
};

void find_initrd(const struct mboot_info* info)
{
    initrd_end = initrd_begin = 0;
    if (!(info->flags & (1 << 3)))
    {
        return;
    }

    struct multiboot_mod_list* module = (void*)(uintptr_t)info->mods_addr;
    for (size_t i = 0; i < info->mods_count; ++i, ++module)
    {
        if (module->mod_end - module->mod_start >= sizeof(struct cpio_header)
                && !strncmp("070701", (void*)(uintptr_t)module->mod_start, 6))
        {
            initrd_begin = module->mod_start;
            initrd_end = module->mod_end;
            break;
        }
    }
}


static uintptr_t align_4(uintptr_t addr)
{
    if (addr % 4)
    {
        addr += 4 - (addr % 4);
    }
    return addr;
}


static uint64_t convert_cpio_num(const char* mem_start)
{
    char buf[10];
    memcpy(buf, mem_start, 8);
    buf[8] = '\0';
    return strtoul(buf, NULL, 16);
}


static void free_memory_rec(uintptr_t start, uintptr_t end)
{
    if (start >= end)
    {
        return;
    }

    for (int i = MAX_ORDER; i >= 0; --i)
    {
        size_t sz = (1ll << i) * PAGE_SIZE;
        uintptr_t aligned_start = start;
        if (aligned_start % sz)
        {
            aligned_start += sz - (aligned_start % sz);
        }
        if (aligned_start + sz > end)
        {
            continue;
        }

        page_free(aligned_start, i);
        free_memory_rec(start, aligned_start);
        free_memory_rec(aligned_start + sz, end);
        break;
    }
}


static void free_initrd_memory(void)
{
    uintptr_t start = initrd_begin;
    uintptr_t end = initrd_end;    

    if (start % PAGE_SIZE)
    {
        start += PAGE_SIZE - (start % PAGE_SIZE);
    }

    free_memory_rec(start, end);
}


static char filename_buffer[2000];

void initramfs(void)
{
    if (initrd_begin >= initrd_end)
    {
        return;
    }

    uintptr_t cur_addr = align_4(initrd_begin) + HIGHER_BASE;
    while (cur_addr < initrd_end + HIGHER_BASE)
    {
        struct cpio_header* cur_entity = (void*)cur_addr;
        uint64_t mode = convert_cpio_num(cur_entity->mode);
        uint64_t namesize = convert_cpio_num(cur_entity->namesize);
        uint64_t filesize = convert_cpio_num(cur_entity->filesize);

        const char* name_start = (char*)(cur_addr + sizeof(struct cpio_header));
        memcpy(filename_buffer, name_start, namesize);
        filename_buffer[namesize] = '\0';
        if (filename_buffer[0] != '/')
        {
            for (int i = namesize + 1; i >= 0; i--)
            {
                filename_buffer[i] = filename_buffer[i - 1];
            }
            filename_buffer[0] = '/';
        }

        void* file_start = (void*)align_4((uintptr_t)name_start + namesize);
        uintptr_t next_entity_start = align_4((uintptr_t)file_start + filesize);

        if (!strcmp(END_OF_ARCHIVE, filename_buffer))
        {
            break;
        }

        if (S_ISDIR(mode))
        {
            int res = mkdir(filename_buffer);
            BUG_ON(res == FS_ERROR);
        }
        else if (S_ISREG(mode))
        {
            int fd = open(filename_buffer);
            BUG_ON(fd == FS_ERROR);
            write(fd, 0, file_start, filesize);
            close(fd);
        }

        cur_addr = next_entity_start;
    }

    dump_filesystem();
    free_initrd_memory();
}
