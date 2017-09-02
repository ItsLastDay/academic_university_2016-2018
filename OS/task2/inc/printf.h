#ifndef __PRINTF_H__
#define __PRINTF_H__

#include <stddef.h>
#include <stdarg.h>
#include <stdint.h>
#include <serial_port.h>

int vprintf(const char *, va_list);

int printf(const char *, ...);

int vsnprintf(char *, size_t, const char *, va_list);

int snprintf(char *, size_t, const char *, ...);


#endif
