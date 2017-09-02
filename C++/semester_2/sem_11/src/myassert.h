#pragma once

#include <iostream>
#include <ciso646>


#ifndef CONFIG_DEBUG
#error "CONFIG_DEBUG not defined"
#elif CONFIG_DEBUG == 0
#define myassert(expr)          \
        do {                    \
            if (0)              \
            {                   \
                (void)(expr);   \
            }                   \
        } while (0)  
#elif CONFIG_DEBUG != 0
#define myassert(expr)      \
        do {                \
            if (not (expr)) {   \
                std::cout << __FUNCTION__ << " " << __FILE__ << ":" << __LINE__ << \
                    " Assertion '" << #expr << "' failed" << std::endl; \
                exit(2); \
            }       \
        } while (0)

#endif // CONFIG_DEBUG
