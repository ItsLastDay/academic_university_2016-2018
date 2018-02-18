#include <iostream>

extern "C" { int sum(int, int); }

int main()
{
    using namespace std;
    cout << sum(10, 20) << endl;
    return 0;
}
