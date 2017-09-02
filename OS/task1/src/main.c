static void qemu_gdb_hang(void)
{
#ifdef DEBUG
	static volatile int wait = 1;

	while (wait);
#endif
}

#include <desc.h>
#include <serial_port.h>
#include <pit.h>
#include <interrupt.h>
#include <printf.h>

void init_hardware()
{
    init_serial_port();

    init_idt();

    init_interrupt_controllers();

    init_pit();
}

void main(void)
{
	qemu_gdb_hang();

        init_hardware();

	while (1);
}
