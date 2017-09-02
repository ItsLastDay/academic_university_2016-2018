#include <string.h>

size_t strlen(const char *str)
{
    size_t answer = 0;

    while (*str)
    {
        ++answer;
        ++str;
    }

    return answer;
}

void* memcpy(void* dst, void* src, size_t size)
{
    char* to = dst;
    const char* from = src;
    
    if (from <= to)
    {
        while (size > 0)
        {
            to[size - 1] = from[size - 1];
            --size;
        }
    }
    else
    {
        for (size_t i = 0; i < size; ++i)
        {
            to[i] = from[i];
        }
    }

    return dst;
}
