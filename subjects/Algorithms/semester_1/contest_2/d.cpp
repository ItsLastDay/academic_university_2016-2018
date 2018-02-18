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

const int MAXN = 1e4 + 12;

bool dp[MAXN][MAXN];

int main()
{   
    cin.sync_with_stdio(false);
    int n, cur;
    int total_sum = 0;
    cin >> n;
    dp[0][0] = true;
    for (int i = 1; i <= n; i++)
    {
        cin >> cur;
        total_sum += cur;
        for (int j = 0; j < MAXN; j++)
        {
            dp[i][j] |= dp[i - 1][j];
            if (j + cur < MAXN)
                dp[i][j + cur] |= dp[i - 1][j];
        }
    }

    int answer = MAXN * 2;
    for (int weight = 0; weight < MAXN; weight++)
    {
        if (dp[n][weight])
        {
//            cout << weight << endl;
            answer = min(answer, abs(2 * weight - total_sum));
        }
    }

    cout << answer << endl;
    return 0;
}
