#ifndef __SERIAL_PORT_H__
#define __SERIAL_PORT_H__

static const int SERIAL_BASE_ADDR = 0x3f8;

void init_serial_port();

void write_byte(char);

#endif
