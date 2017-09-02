#ifndef __STRING_H__
#define __STRING_H__

#include <stddef.h>
#include <stdint.h>

uint8_t strcmp(const char* str1, const char* str2);
uint8_t strncmp(const char* str1, const char* str2, size_t max_chars);
size_t strlen(const char *str);
void *memcpy(void *dst, const void *src, size_t size);
void *memset(void *dst, int fill, size_t size);

#endif /*__STRING_H__*/
