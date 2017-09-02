#include <bits/stdc++.h>
using namespace std;
using namespace std::chrono;
#define DBG(x) cout << #x << " = " << (x) << endl;
/*
Compile:
    -Wall -Wextra -pedantic -std=c++14 -O2 -Wshadow -Wformat=2 -Wfloat-equal -Wconversion -Wlogical-op -Wcast-qual -Wcast-align -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -D_FORTIFY_SOURCE=2 -fsanitize=address -fsanitize=undefined -fno-sanitize-recover -fstack-protector 

    -std=c++14 -O2
 
Check for memory usage:
    valgrind --tool=massif
*/

using compl_t = complex<double>;

vector<compl_t> fft(vector<compl_t> input, compl_t base)
{
    int n = input.size();
    if (n == 1)
    {
        return input;
    }

    vector<compl_t> odd(n / 2), even(n / 2);
    for (int i = 0; i < n; i++)
    {
        if (not (i & 1))
        {
            odd[i / 2] = input[i];
        }
        else
        {
            even[i / 2] = input[i];
        }
    }

    auto res_odd = fft(move(odd), base * base);
    auto res_even = fft(move(even), base * base);

    compl_t base_cur = base;
    for (int i = 0; i < n / 2; i++)
    {
        input[i] = res_even[i] + base_cur * res_odd[i];
        input[i + n / 2] = res_even[i] - base_cur * res_odd[i];
        base_cur *= base;
    }

    return input;
}

int powmod(int x, int n, int m)
{
    int res = 1 % m;
    while (n)
    {
        if (n & 1)
            res = res * x % m;
        x = x * x  % m;
        n >>= 1;
    }

    return res;
}

const int MAXN = 1 << 21;

int cnt_basic[MAXN];

int main()
{   
    cin.sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    cin >> n >> m;
    for (int i = 1; i < m; i++)
    {
        cnt_basic[powmod(i, n, m)]++;
    }

    vector<compl_t> a(MAXN);
    for (int i = 1; i < m; i++)
    {
        a[i] = cnt_basic[i];
        cout << i << " " << cnt_basic[i] << endl;
    }
    cout << endl;

    auto base = std::polar(1.0, M_PI / MAXN);
    auto res = fft(a, base);

    vector<compl_t> qq(MAXN);
    for (int i = 0; i < MAXN; i++)
        qq[i] = res[i] * res[i];

    auto inv = fft(qq, pow(base, -1));
    for (int i = 0; i < MAXN; i++)
        inv[i] /= MAXN;

    for (int i = 1; i < 2 * m; i++)
    {
        cout << i << " " << inv[i].real() << " " << inv[i].imag() << endl;
    }
    return 0;
}
