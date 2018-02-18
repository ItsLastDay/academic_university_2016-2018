#include "libexample.h"
#include <cstdio>

void libexample_hello(void) noexcept
{
    std::printf("Hello from example lib!\n");
}
