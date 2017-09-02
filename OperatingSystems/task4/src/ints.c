#include <ints.h>
#include <i8259a.h>
#include <memory.h>
#include <desc.h>
#include <print.h>
#include <backtrace.h>

#define IDT_PRESENT	((uint64_t)1 << 47)
#define IDT_INT_GATE	((uint64_t)14 << 40)
#define IDT_TRAP_GATE	((uint64_t)15 << 40)
#define IDT_USER	((uint64_t)3 << 45)
#define IDT_ENTRIES	256

#define IRQS		16
#define ERRORS		32
#define PIC_OFFSET	ERRORS


struct idt_entry {
	uint64_t low;
	uint64_t high;
} __attribute__((packed));

static irq_handler_t irq_handler[IRQS];
static error_handler_t error_handler[ERRORS];


static void idt_entry_setup(struct idt_entry *entry, uint16_t selector,
			uint64_t handler, uint64_t flags)
{
	entry->low = (handler & 0xFFFFul) | ((uint64_t)selector << 16)
		| ((handler & 0xFFFF0000ul) << 32) | flags;
	entry->high = (handler >> 32) & 0xFFFFFFFFul;
}

static void unexpected_interrupt(struct frame *frame)
{
	printf("Unexpected interrupt %d at 0x%lx\n", (int)frame->intno,
				frame->rip);
	printf("Backtrace Begin:\n");
	__backtrace(frame->rbp, stack_begin(), stack_end());
	printf("Backtrace End.\n");
	while (1);
}

static void call_error_handler(int error, struct frame *frame)
{
	error_handler_t handler = error_handler[error];

	if (!handler)
		unexpected_interrupt(frame);
	handler(error, frame);
}

static void call_irq_handler(int irq, struct frame *frame)
{
	irq_handler_t handler = irq_handler[irq];

	if (!handler)
		unexpected_interrupt(frame);

	/* We can ack right away since we use Interrupt Gates and so
	 * interrupts are disabled on CPU. */
	pic_ack(irq);
	handler(irq, frame);
}

void __int_handler(struct frame *frame)
{
	if (frame->intno < ERRORS)
		call_error_handler(frame->intno, frame);
	else if (frame->intno < ERRORS + IRQS)
		call_irq_handler(frame->intno - ERRORS, frame);
	else
		unexpected_interrupt(frame);
}

void register_error_handler(int error, error_handler_t handler)
{
	error_handler[error] = handler;
}

void register_irq_handler(int irq, irq_handler_t handler)
{
	irq_handler[irq] = handler;
}

void activate_irq(int irq)
{
	pic_unmask(irq);
}

void deactivate_irq(int irq)
{
	pic_mask(irq);
}

static void idt_setup(void)
{
	static const uint64_t flags = IDT_PRESENT | IDT_INT_GATE;
	static struct idt_entry idt[IDT_ENTRIES];
	extern uint64_t __int_entry[];

	/* All idt entries by default set to Interrupt Gate type, so that
	 * when interrupt happens cpu before calling interrupt handler
	 * disables interrupts until iret instruction executed. */
	for (int i = 0; i != IDT_ENTRIES; ++i)
		idt_entry_setup(&idt[i], KERNEL_CS, __int_entry[i], flags);

	struct desc_table_ptr ptr = {
		.size = sizeof(idt) - 1,
		.addr = (uint64_t)idt
	};
	write_idtr(&ptr);
}

void ints_setup(void)
{
	idt_setup();
	pic_setup(PIC_OFFSET);
}
