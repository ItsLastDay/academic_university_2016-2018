#include <ioport.h>
#include <serial_port.h>

void init_serial_port()
{
    // Set speed.
    out8(SERIAL_BASE_ADDR + 3, 1u << 7); 
    out8(SERIAL_BASE_ADDR + 0, 12);
    out8(SERIAL_BASE_ADDR + 1, 0);

    // Set frame format.
    out8(SERIAL_BASE_ADDR + 3, 1 | 2);

    // Disable interruptions.
    out8(SERIAL_BASE_ADDR + 1, 0);
}

void write_byte(char b)
{
    // Check that we can write.
    while (!(in8(SERIAL_BASE_ADDR + 5) & (1 << 5)))
        ;
    out8(SERIAL_BASE_ADDR + 0, b);
}
