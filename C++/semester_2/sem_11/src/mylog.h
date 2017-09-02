#pragma once


#ifndef CONFIG_LOG_LEVEL
#error "Log level must be defined"
#endif

#define DEBUG 0
#define INFO 1
#define WARN 2
#define ERROR 3


#define LOG(LEVEL, ...) \
    do {                        \
        if (LEVEL >= CONFIG_LOG_LEVEL)  \
        {                       \
            printf("%s:%d:%s: ", __FILE__, __LINE__, __FUNCTION__);    \
            printf(__VA_ARGS__);   \
            printf("\n");          \
        }                       \
    } while (0)


