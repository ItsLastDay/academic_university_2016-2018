#include <functional>
#include <iostream>
#include <vector>

using namespace std;

using vector_trans_callback = function<int(int)>;

int mul_by_2(int x)
{
    return x * 2;
}

int my_sum(vector<int> &vec, vector_trans_callback callback)
{
    int sum = 0;

    for (auto it = vec.begin(); it != vec.end(); ++it)
    {
        sum += callback(*it);
    }

    return sum;
}

int main()
{
    vector<int> vec = {1, 2, 3, 4, 5};
    cout << my_sum(vec, mul_by_2) << endl;
    return 0;
}
