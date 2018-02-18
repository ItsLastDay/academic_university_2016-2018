#include <iostream>
#include <unistd.h>

#define U_MS100 100000

static void work1x()
{
    usleep(1 * U_MS100);
}

static void work2x()
{
    usleep(2 * U_MS100);
}

static void work3x()
{
    usleep(3 * U_MS100);
}

int main()
{
    for(size_t i = 0; i < 10; ++i) {
        work1x();
        work2x();
        work3x();
    }
    return 0;
}
