#include <ioport.h>
#include <pit.h>

void init_pit()
{
    int command = 0;

    // Set both bytes of freq divisor.
    command |= 3 << 4;

    // Mode 2: rate generator.
    command |= 2 << 1;

    out8(PIT_CMD_PORT, command);

    // Set freq divisor.
    out8(PIT_DATA_PORT, 255);
    out8(PIT_DATA_PORT, 255);
}
