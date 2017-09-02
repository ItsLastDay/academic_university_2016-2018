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

bool dp[2][MAXN];

int main()
{   
    cin.sync_with_stdio(false);
    string pattern, str;
    cin >> pattern >> str;

    int n = pattern.size();
    int m = str.size();

    dp[0][0] = true;

    for (int i = 0; i < n; i++)
    {
        memset(dp[1 - (i & 1)], false, sizeof(dp[1 - (i & 1)]));
        for (int j = 0; j <= m; j++)
        {
            if (dp[i&1][j] == false)
                continue;
            char pat = pattern[i];
            if (pat == '*')
            {
                dp[i&1][j + 1] = true;
                dp[1 - (i & 1)][j + 1] = true;
                dp[1 - (i & 1)][j] = true;
            }
            else if(pat == '?')
            {
                dp[1 - (i & 1)][j + 1] = true;
            }
            else
            {
                if (j < m and pat == str[j])
                    dp[1 - (i & 1)][j + 1] = true;
            }
        }
    }

    cout << (dp[n&1][m] ? "YES" : "NO") << endl;
    return 0;
}



