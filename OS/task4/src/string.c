#include <string.h>


uint8_t strcmp(const char*str1, const char* str2)
{
    return strncmp(str1, str2, -1);
}


uint8_t strncmp(const char* str1, const char* str2, size_t max_chars)
{
    size_t num_read_chars = 0;

    while (*str1 && *str2 && num_read_chars < max_chars)
    {
        if (*str1 != *str2)
        {
            break;
        }
        ++str1;
        ++str2;
        ++num_read_chars;
    }

    if (num_read_chars == max_chars || *str1 == *str2)
    {
        return 0;
    }

    return *str1 < *str2 ? -1 : 1;
}

size_t strlen(const char *str)
{
	const char *begin = str;

	while (*str++);
	return str - begin - 1;
}

void *memcpy(void *dst, const void *src, size_t size)
{
	char *to = dst;
	const char *from = src;

	while (size--)
		*to++ = *from++;
	return dst;
}

void *memset(void *dst, int fill, size_t size)
{
	char *to = dst;

	while (size--)
		*to++ = fill;
	return dst;
}
