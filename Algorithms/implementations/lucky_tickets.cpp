#include <bits/stdc++.h>
using namespace std;

const int MAXN = 100;

long long dp[MAXN][MAXN * 9];

void precalc()
{
    dp[0][0] = 1;
    for (int i = 1; i < MAXN; i++) 
    {
        for (int j = 0; j <= i * 9; j++) 
        {
            for (int k = 0; k < 10; k++)
            {
                if (j - k < 0)
                {
                    continue;
                }
                dp[i][j] += dp[i - 1][j - k];
            }
        }
    }
}

long long get_lucky_tickets(int n)
{
    assert(n < MAXN);
    long long tickets = 0;

    for (int i = 0; i <= 9 * n; i++)
    {
        tickets += dp[n][i] * dp[n][i];
    }

    return tickets;
}

void gen(int pos, int sum, vector<long long> &cnt)
{
    if (pos == 0)
    {
        cnt[sum]++;
    } 
    else
    {
        for (int i = 0; i < 10; i++)
        {
            gen(pos - 1, sum + i, cnt);
        }
    }
}

long long stupid_sol_tickets(int n)
{
    vector<long long> cnt_sum_ways(n * 9 + 1, 0);
    gen(n, 0, cnt_sum_ways);

    long long tickets = 0;
    for (int i = 0; i <= n * 9; i++)
    {
        assert(cnt_sum_ways[i] == dp[n][i]);
        tickets += cnt_sum_ways[i] * cnt_sum_ways[i];
    }

    return tickets;
}

int main()
{
    precalc();

    for (int i = 0; i <= 10; i++)
    {
        cout << i << " " << get_lucky_tickets(i) << endl;
        cout << i << " " << stupid_sol_tickets(i) << endl;
    }
    return 0;
}
