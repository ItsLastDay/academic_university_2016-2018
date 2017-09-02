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

const int MAXN = 1050;
const int MOD = 9999991;

int dp[MAXN][MAXN];

int main()
{   
    cin.sync_with_stdio(false);
    int n, k;
    cin >> k >> n;
    dp[n][0] = 1;

    for (int i = 1; i <= k; i++)
    {
        for (int j = 0; j <= n; j++) 
        {
            for (int t = 3; t <= 13; t++)
            {
                dp[j][i] += dp[j + t][i - 1];
                while (dp[j][i] >= MOD)
                    dp[j][i] -= MOD;
            }
        }
    }

    cout << dp[0][k] << endl;
    return 0;
}
