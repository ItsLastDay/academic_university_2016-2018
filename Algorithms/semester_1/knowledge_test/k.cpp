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

int a[1010];
int b[1010];

long long solve()
{
    int n;
    cin >> n;
    for(int i = 0; i < n; i++)
        cin >> a[i];
    for(int i = 0; i < n; i++)
        cin >> b[i];
    sort(a, a + n);
    sort(b, b + n);

    long long res = 0;
    for(int i = 0; i < n; i++)
        res += a[i] * static_cast<long long>(b[n - i - 1]);
    return res;
}

int main()
{
    cin.sync_with_stdio(false);
    int n;
    cin >> n;
    for(int i = 0; i < n; i++)
        cout << "Case #" << i + 1 << ": " << solve() << "\n";
    return 0;
}
