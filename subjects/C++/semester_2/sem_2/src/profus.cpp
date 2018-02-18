#include <iostream>

size_t fib(size_t n)
{
    size_t prev, prevprev, current, i;
    if (n == 0)
        return 0;
    if (n == 1)
        return 1;

    for(prevprev = 0, prev = 1, current = 1, i = 2; i < n; ++i)
    {
        prevprev = prev;
        prev = current;
        current = prevprev + prev;
    }
    return current;
}

static void work1x()
{
    fib(10000000);
}

static void work2x()
{
    fib(20000000);
}

static void work3x()
{
    fib(30000000);
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
