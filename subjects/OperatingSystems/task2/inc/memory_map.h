#ifndef __MEMORY_MAP_H__
#define __MEMORY_MAP_H__
#include <stdint.h>
#include <stddef.h>


#define MMAP_VALIDITY_BIT (1 << 6)


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

struct multiboot_info
 {
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
 };
 typedef struct multiboot_info multiboot_info_t;

struct multiboot_mmap_entry
 {
   multiboot_uint32_t size;
   multiboot_uint64_t addr;
   multiboot_uint64_t len;
 #define MULTIBOOT_MEMORY_AVAILABLE              1
 #define MULTIBOOT_MEMORY_RESERVED               2
   multiboot_uint32_t type;
 } __attribute__((packed));
 typedef struct multiboot_mmap_entry multiboot_memory_map_t;


// https://github.com/google/rekall/blob/354d1a6e6ba12d296b995eca8d20b6803b88cb00/tools/linux/lmap/minpmem/pte_mmap.h
struct PML4Entry
{
    uint64_t present        : 1;
    uint64_t rw             : 1;
    uint64_t user           : 1;
    uint64_t write_through  : 1;
    uint64_t cache_disable  : 1;
    uint64_t accessed       : 1;
    uint64_t ignored_1      : 1;
    uint64_t reserved_1     : 1;
    uint64_t ignored_2      : 4;
    uint64_t pdpt_p         :40;
    uint64_t ignored_3      :11;
    uint64_t xs             : 1;
} __attribute__((packed));


// Remastered from the above link using Intel manual.
struct PDPTEntry
{
    uint64_t present        : 1;
    uint64_t rw             : 1;
    uint64_t user           : 1;
    uint64_t write_through  : 1;
    uint64_t cache_disable  : 1;
    uint64_t accessed       : 1;
    uint64_t dirty          : 1;
    uint64_t page_size      : 1;
    uint64_t global         : 1;
    uint64_t ignored_2      : 3;
    uint64_t pat            : 1;
    uint64_t reserved_1     :17;
    uint64_t phys_addr      :22;
    uint64_t ignored_3      : 7;
    uint64_t prot_key       : 4;
    uint64_t xd             : 1;
} __attribute__((packed));

void init_memory();

#endif
