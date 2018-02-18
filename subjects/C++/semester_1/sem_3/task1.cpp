#include <iostream>

void f()
{
    using namespace std;
    int t;
    cout << "f stack addresses:" << endl;
    cout << &t << endl;
    f();
}

int main()
{
    using namespace std;

    int* x = new int (1);
    int* y = new int (2);

    cout << "Heap addresses:" << endl;
    cout << x << endl << y << endl;

    cout << "Main stack addresses:" << endl;
    cout << &x << endl << &y << endl;

    f();
    return 0;
}
