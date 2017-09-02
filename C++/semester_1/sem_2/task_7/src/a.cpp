#include "a.h"

const int A1 = 10;

#ifndef HAPPY_DEBUG
int add(int a, int b)
{
    return a + b;
}
#else
int add(int a, int b)
{
    return a + b + 1;
}
#endif

void change_a()
{
    int& a = global_a();
    a++;
}
