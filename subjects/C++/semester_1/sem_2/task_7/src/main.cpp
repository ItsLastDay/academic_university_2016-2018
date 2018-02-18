#include <iostream>
#include "a.h"

int main()
{
    using namespace std;
    cout << global_a() << endl;
    change_a();
    cout << global_a() << endl;
    return 0;
}
