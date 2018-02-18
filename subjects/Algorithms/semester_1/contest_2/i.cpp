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

const int MAXN = 1 << 10;

int val[MAXN];
bool was[MAXN][MAXN];
int dp[MAXN][MAXN];
int max_lev;

int get(int v, int k, int lev)
{
    if (k < 0 or lev > max_lev)
        return 0;

    if (was[v][k])
        return dp[v][k];

    if (lev == max_lev)
    {
        was[v][k] = true;
        dp[v][k] = val[v];
        return dp[v][k];
    }

    was[v][k] = true;

    dp[v][k] = max(dp[v][k], get(2 * v + 1, k - 1, lev + 1));
    dp[v][k] = max(dp[v][k], get(2 * v + 2, k - 1, lev + 1));

    for (int i = 0; i <= k - 2; i++)
    {
        dp[v][k] = max(dp[v][k], get(2 * v + 1, i, lev + 1) + 
                get(2 * v + 2, k - 2 - i, lev + 1));
    }

    dp[v][k] += val[v];

    return dp[v][k];
}

int main()
{   
    cin.sync_with_stdio(false);
    cin.tie(NULL);
    cin >> max_lev;
    for (int i = 0; i < (1 << max_lev) - 1; i++)
        cin >> val[i];
    
    int num;
    cin >> num;
    cout << get(0, num, 1) << endl;
    return 0;
}
