#ifndef __SERIAL_H__
#define __SERIAL_H__

#include <stddef.h>

void serial_setup(void);
void serial_putchar(int c);
void serial_write(const char *buf, size_t size);

#endif /*__SERIAL_H__*/
