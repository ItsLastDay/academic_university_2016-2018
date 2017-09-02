#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__

#include <stdint.h>

static const int PIC_CMD_MASTER_PORT = 0x20,
      PIC_DATA_MASTER_PORT = 0x21,
      PIC_CMD_SLAVE_PORT = 0xA0,
      PIC_DATA_SLAVE_PORT = 0xA1;

static const int PIC_MASTER_IDT_START = 32,
      PIC_SLAVE_IDT_START = 40;

static const int PIC_NUM_LEGS = 8;

void init_idt();

void init_interrupt_controllers();

struct IDTEntry
{
    uint16_t offset_low;
    uint16_t seg_selector;
    uint16_t type;
    uint16_t offset_mid;
    uint32_t offset_high;
    uint32_t smth_null;
} __attribute__((packed));

struct InterruptFrame
{
    uint64_t r11; 
    uint64_t r10; 
    uint64_t r9; 
    uint64_t r8; 
    uint64_t rsi; 
    uint64_t rdi; 
    uint64_t rdx; 
    uint64_t rcx; 
    uint64_t rax; 

    uint64_t vector;
    uint64_t error_code;
    uint64_t old_rip;
    uint64_t old_cs;
    uint64_t old_rflags;
    uint64_t old_rsp;
    uint64_t old_ss;
} __attribute__((packed));

#endif
