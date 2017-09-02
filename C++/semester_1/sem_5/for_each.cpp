#include <vector>
#include <iostream>
#include <functional>

using namespace std;

using vector_int_callback = std::function<void(int&)>;

void square_int(int& x)
{
    x *= x;
}

void print_int(int &x)
{
    cout << x << " ";
}

void my_for_each(vector<int> &vec, vector_int_callback callback)
{
    for (auto it = vec.begin(); it != vec.end(); ++it) 
    {
        callback(*it);
    }
}

int main()
{
    vector<int> vec = {1, 2, 3, 4, 5};
    my_for_each(vec, square_int);
    my_for_each(vec, print_int);

    cout << endl;

    my_for_each(vec, [](int &x) { x *= x - 1; });
    my_for_each(vec, print_int);
    return 0;
}
