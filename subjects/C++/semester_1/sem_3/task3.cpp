#include <iostream>
#include <cstdlib>
#include <cstring>

const char* concat_arrays(
        const char a[],
        const size_t a_size,
        const char b[],
        const size_t b_size,
        size_t &concat_size)
{
    concat_size = a_size + b_size - 1;
    char *res = new char[concat_size];

    memcpy(res, a, (a_size - 1) * sizeof(*a));
    memcpy(res + a_size - 1, b, b_size * sizeof(*b));

    return res;
}

int main()
{
    using namespace std;
    const char a[] = "hello!";
    const char b[] = "my god!";

    size_t concat_sz;
    auto t = concat_arrays(a, sizeof(a), b, sizeof(b), concat_sz);
    cout << concat_sz << endl << t << endl;
    return 0;
}
