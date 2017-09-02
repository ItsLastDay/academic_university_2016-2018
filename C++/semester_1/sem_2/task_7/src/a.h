#pragma once

int add(int, int);
void change_a();
extern const int A1;
const int A2 = 20;


inline int& global_a()
{
    static int a = add(A1, A2);
    return a;
}


