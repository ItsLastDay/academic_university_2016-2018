#ifndef __INTS_H__
#define __INTS_H__

#include <stdint.h>

struct frame {
	uint64_t r15;
	uint64_t r14;
	uint64_t r13;
	uint64_t r12;
	uint64_t rbp;
	uint64_t rbx;
	uint64_t r11;
	uint64_t r10;
	uint64_t r9;
	uint64_t r8;
	uint64_t rax;
	uint64_t rcx;
	uint64_t rdx;
	uint64_t rsi;
	uint64_t rdi;
	uint64_t intno;
	uint64_t error;
	uint64_t rip;
	uint64_t cs;
	uint64_t rflags;
	uint64_t rsp;
	uint64_t ss;
} __attribute__((packed));

typedef void (*irq_handler_t)(int irq, struct frame *frame);
typedef void (*error_handler_t)(int error, struct frame *frame);

static inline void disable_ints(void)
{ __asm__ volatile ("cli" : : : "cc"); }

static inline void enable_ints(void)
{ __asm__ volatile ("sti" : : : "cc"); }

static inline int ints_enabled(void)
{
	static const unsigned long RFLAGS_IF = (1ul << 9);
	unsigned long flags;

	__asm__ volatile ("pushfq ; pop %0" : "=rm"(flags) : : "memory");
	return (flags & RFLAGS_IF) != 0;
}

void ints_setup(void);
void register_error_handler(int error, error_handler_t handler);
void register_irq_handler(int irq, irq_handler_t handler);
void activate_irq(int irq);
void deactivate_irq(int irq);

#endif /*__INTS_H__*/
