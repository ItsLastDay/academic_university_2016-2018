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
#include <memory_map.h>
#include <hashtable.h>
#include <slab_alloc.h>
#include <mymalloc.h>

void init_hardware()
{
    init_serial_port();

    init_idt();

    init_interrupt_controllers();

    init_pit();
}

void main(uintptr_t loader_info_head)
{
	qemu_gdb_hang();

        init_hardware();

        init_memory(loader_info_head);
	while (1);
}
