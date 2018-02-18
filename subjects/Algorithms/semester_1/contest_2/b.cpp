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

#define prev asdasdasd
const int MAXN = 110;

enum move_case
{
    I, J, IJ, NO
};

move_case prev[MAXN][MAXN];
char prev_char[MAXN][MAXN];
int dp[MAXN][MAXN];

void upd(int &cur, int val, move_case &cmv, move_case mv, char &pc, char cc)
{
    if (cur == -1 or val < cur)
    {
        cmv = mv;
        cur = val;
        pc = cc;
    }
}

string get(string const& s, string const& p)
{
    int n = s.size(), m = p.size();
    for (int i = 0; i <= n; i++)
        for (int j = 0; j <= m; j++)
        {
            dp[i][j] = -1;
            prev[i][j] = NO;
            prev_char[i][j] = 0;
        }

    dp[0][0] = 0;

    for (int i = 0; i <= n; i++)
    {
        for (int j = 0; j <= m; j++)
        {
            if (i == n and j == m)
                continue;
            if (dp[i][j] == -1)
                continue;

            if (i < n)
            {
                char pat = s[i];
                bool is_not_star = j == m or p[j] != '*';
                if (pat == '*')
                {
                    if (j < m)
                    {
                    upd(dp[i + 1][j + 1], dp[i][j] + is_not_star, prev[i + 1][j + 1], IJ,
                            prev_char[i + 1][j + 1], is_not_star ? p[j] : 0);
                    upd(dp[i][j + 1], dp[i][j] + is_not_star, prev[i][j + 1], J,
                            prev_char[i][j + 1], is_not_star ? p[j] : 0);
                    }
                    upd(dp[i + 1][j], dp[i][j], prev[i + 1][j], I,
                            prev_char[i + 1][j], 0);
                }
                else if (pat == '?')
                {
                    if (j < m)
                    {
                    upd(dp[i + 1][j + 1], dp[i][j] + 1, prev[i + 1][j + 1], IJ,
                            prev_char[i + 1][j + 1], is_not_star ? p[j] : pat);
                    }
                }
                else 
                {
                    if (j < m and p[j] == pat)
                    {
                        upd(dp[i + 1][j + 1], dp[i][j] + 1, prev[i + 1][j + 1], IJ,
                                prev_char[i + 1][j + 1], pat);
                    }
                }
            }

            if (j < m)
            {
                char pat = p[j];
                bool is_not_star = i == n or s[i] != '*';
                if (pat == '*')
                {
                    if (i < n)
                    {
                    upd(dp[i + 1][j + 1], dp[i][j] + is_not_star, prev[i + 1][j + 1], 
                            IJ, prev_char[i + 1][j + 1], is_not_star ? s[i] : 0);
                    upd(dp[i + 1][j], dp[i][j] + is_not_star, prev[i + 1][j], I,
                            prev_char[i + 1][j], is_not_star ? s[i] : 0);
                    }
                    upd(dp[i][j + 1], dp[i][j], prev[i][j + 1], J,
                            prev_char[i][j + 1], 0);
                }
                else if (pat == '?')
                {
                    if (i < n)
                    {
//                        cout << dp[i + 1][j + 1] << " " << dp[i][j] + 1 << endl;
                    upd(dp[i + 1][j + 1], dp[i][j] + 1, prev[i + 1][j + 1], IJ,
                            prev_char[i + 1][j + 1], is_not_star ? s[i] : pat);
//                    cout << prev_char[i + 1][j + 1] << endl;
                    }
                }
                else 
                {
                    if (i < n and s[i] == pat)
                    {
                        upd(dp[i + 1][j + 1], dp[i][j] + 1, prev[i + 1][j + 1], IJ,
                                prev_char[i + 1][j + 1], pat);
                    }
                }
            }
        }
    }

    if (dp[n][m] == -1)
        return "NO";

    string answer = "";

    int cur_i = n, cur_j = m;
    while(cur_i > 0 or cur_j > 0)
    {
//        cout << cur_i << " tt " << cur_j << endl;
        int now_len = dp[cur_i][cur_j];
        int next_i = cur_i, next_j = cur_j;

        switch (prev[cur_i][cur_j])
        {
            case I:
                next_i--;
                break;
            case J:
                next_j--;
                break;
            case IJ:
                next_i--;
                next_j--;
                break;
            default:
                assert(false);
        }
        int prev_len = dp[next_i][next_j];
        /*
        cout << next_i << " " << next_j << endl;
        cout << now_len << endl;
        cout << prev_len << endl;
        */

        if (prev_len != now_len)
        {
            assert(prev_char[cur_i][cur_j] != 0);
            if (prev_char[cur_i][cur_j] == '?')
                prev_char[cur_i][cur_j] = 'a';
            answer += prev_char[cur_i][cur_j];
        }

        cur_i = next_i;
        cur_j = next_j;
    }

    reverse(answer.begin(), answer.end());
    return answer;
}


int main()
{   
    cin.sync_with_stdio(false);
    int n;
    cin >> n;
    cin.ignore(1);
    for (int i = 0; i < n; i++)
    {
        string s1, s2;
        getline(cin, s1);
        getline(cin, s2);
        cout << get(s1, s2) << "\n";
    }
    return 0;
}
