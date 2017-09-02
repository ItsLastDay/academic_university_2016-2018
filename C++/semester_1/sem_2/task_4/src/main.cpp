#include <iostream>
#include "a.h"

int main()
{
    extern int global_a;
    using namespace std;
    cout << global_a << endl;
    return 0;
}
