#include <i8259a.h>
#include <ioport.h>

#define PIC_MASTER_CMD	0x20
#define PIC_MASTER_DATA	0x21
#define PIC_SLAVE_CMD	0xA0
#define PIC_SLAVE_DATA	0xA1

#define PIC_SLAVE_IRQ		2
#define PIC_SLAVE_IRQ_BIT	(1 << PIC_SLAVE_IRQ)
#define PIC_IRQS		8

#define PIC_ACK_CMD		0x60
#define PIC_ICW1_EXPECT_ICW4	(1 << 0)
#define PIC_ICW1_INIT		(1 << 4)
#define PIC_ICW4_8086		(1 << 0)

/* By default mask all PIC inputs except 2-nd input of the master,
 * since it's used to connect slave chip. */
static unsigned pic_irq_mask = ~((unsigned)PIC_SLAVE_IRQ_BIT);

void pic_setup(int offset)
{
	out8(PIC_MASTER_CMD, PIC_ICW1_EXPECT_ICW4 | PIC_ICW1_INIT);
	out8(PIC_SLAVE_CMD, PIC_ICW1_EXPECT_ICW4 | PIC_ICW1_INIT);
	out8(PIC_MASTER_DATA, offset);
	out8(PIC_SLAVE_DATA, offset + PIC_IRQS);
	out8(PIC_MASTER_DATA, PIC_SLAVE_IRQ_BIT);
	out8(PIC_SLAVE_DATA, PIC_SLAVE_IRQ);
	out8(PIC_MASTER_DATA, PIC_ICW4_8086);
	out8(PIC_SLAVE_DATA, PIC_ICW4_8086);

	out8(PIC_MASTER_DATA, pic_irq_mask & 0xFFu);
	out8(PIC_SLAVE_DATA, (pic_irq_mask >> 8) & 0xFFu);
}

void pic_mask(int irq)
{
	pic_irq_mask |= 1u << irq;
	if (irq < 8)
		out8(PIC_MASTER_DATA, pic_irq_mask & 0xFFu);
	else
		out8(PIC_SLAVE_DATA, (pic_irq_mask >> 8) & 0xFFu);
}

void pic_unmask(int irq)
{
	pic_irq_mask &= ~(1u << irq);
	if (irq < 8)
		out8(PIC_MASTER_DATA, pic_irq_mask & 0xFFu);
	else
		out8(PIC_SLAVE_DATA, (pic_irq_mask >> 8) & 0xFFu);
}

void pic_ack(int irq)
{
	if (irq >= 8) {
		out8(PIC_SLAVE_CMD, PIC_ACK_CMD + (irq & 7));
		out8(PIC_MASTER_CMD, PIC_ACK_CMD + PIC_SLAVE_IRQ);
	} else {
		out8(PIC_MASTER_CMD, PIC_ACK_CMD + irq);
	}
}
