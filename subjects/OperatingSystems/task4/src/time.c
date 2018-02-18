#include <time.h>
#include <ints.h>
#include <ioport.h>
#include <print.h>
#include <threads.h>

#define PIT_CMD		0x43
#define PIT_CH0_DATA	0x40
#define PIT_FREQ	1193180ul
#define PIT_IRQ		0

/*
 * Timer/Counter Control Register Format:
 * +-----+-----+-----+-----+-----+-----+-----+-----+
 * | sc1 | sc0 | rw1 | rw0 | M2  | M1  | M0  | BCD |
 * +-----+-----+-----+-----+-----+-----+-----+-----+
 */
#define PIT_MODE(x)	(((x) & 7) << 1)
#define PIT_BYTES(x)	(((x) & 3) << 4)
#define PIT_CHANNEL(x)	(((x) & 3) << 6)

#define PIT_LO		1
#define PIT_HI		2
#define PIT_RATE	2

static unsigned long pit_divisor(unsigned long freq)
{
	return PIT_FREQ / freq;
}

static void pit_set_frequency(unsigned long freq)
{
	const unsigned long div = pit_divisor(freq);
	const unsigned char cmd = PIT_MODE(PIT_RATE)
				| PIT_BYTES(PIT_LO | PIT_HI)
				| PIT_CHANNEL(0);

	out8(PIT_CMD, cmd);
	out8(PIT_CH0_DATA, div & 0xff);
	out8(PIT_CH0_DATA, (div >> 8) & 0xff);
}

static unsigned long long jiffies;

static void pit_handler(int irq, struct frame *frame)
{
	(void) irq;
	(void) frame;
	++jiffies;
	schedule();
}

unsigned long long current_time(void)
{ return jiffies; }

void time_setup(void)
{
	register_irq_handler(PIT_IRQ, &pit_handler);
	pit_set_frequency(HZ);
	activate_irq(PIT_IRQ);
}
