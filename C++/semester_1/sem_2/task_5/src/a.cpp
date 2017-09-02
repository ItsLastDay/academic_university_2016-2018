#include "a.h"

const int A1 = 10;

int add(int a, int b)
{
    return a + b;
}

void change_a()
{
    int& a = global_a();
    a++;
}
