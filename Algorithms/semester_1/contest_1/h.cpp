#include <bits/stdc++.h>
using namespace std;
using namespace std::chrono;
#define DBG(x) cout << #x << " = " << x << "\n";
/*
Compile:
    -Wall -Wextra -pedantic -std=c++11 -O2 -Wshadow -Wformat=2 -Wfloat-equal -Wconversion -Wlogical-op -Wcast-qual -Wcast-align -fwhole-program -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -lmcheck -fsanitize=address 

    -std=c++11 -O2
 
Check for memory usage:
    valgrind --tool=massif
*/

const int MAXN = 30100;
const int MD = 1 << 30;

int a[MAXN];

int fast_mod(int x, int mod)
{
    while(x >= mod)
        x -= mod;
    while(x < 0)
        x += mod;
    return x;
}

int main()
{
    cin.sync_with_stdio(false);
    cin.tie(NULL);

    int n, k;
    cin >> n >> k;
    for(int i = 0; i < n; i++)
        cin >> a[i];

    int low = 0, high = n - 1;
    for(int i = 0; i < k; i++) {
        int x = a[low];
        int y = a[high];

        if(x < y)
        {
            low = fast_mod(low + 1, n);
            high = fast_mod(high + 1, n);
            a[high] = fast_mod(x + y, MD);
        }
        else
        {
            low = fast_mod(low - 1, n);
            high = fast_mod(high - 1, n);
            a[low] = fast_mod(y - x, MD);
        }
    }

    do
    {
        cout << a[low] << " ";
        low = fast_mod(low + 1, n);
    } while(low != high);
    if(n != 1)
        cout << a[high] << "\n";
    return 0;
}
