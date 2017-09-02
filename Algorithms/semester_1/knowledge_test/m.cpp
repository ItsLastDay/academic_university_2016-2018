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

const int N = 8;
const int NO_MOVE = 1e7;

int a[N][N];
int dp[N][N][3];
bool was[N][N][3];

bool is_valid(int i, int j)
{
    return i >= 0 and i < N and j >= 0 and j < N;
}

int upd(int cur, int nx, int who_moves)
{
    if(cur == NO_MOVE)
        return nx;
    if(who_moves == 1)
        return max(cur, nx);
    return min(cur, nx);
}

int upd_sum(int ret, int val, int who_moves)
{
    if(ret == NO_MOVE)
        ret = 0;
    if(who_moves == 1)
        return ret - val;
    return ret + val;
}

int get(int i, int j, int who_moves)
{
    if(was[i][j][who_moves])
        return dp[i][j][who_moves];
    was[i][j][who_moves] = true;
    int &ret = dp[i][j][who_moves];

    ret = NO_MOVE;
    if(is_valid(i - 1, j))
        ret = upd(ret, get(i - 1, j, 3 - who_moves), who_moves);
    if(is_valid(i - 1, j + 1))
        ret = upd(ret, get(i - 1, j + 1, 3 - who_moves), who_moves);
    if(is_valid(i, j + 1))
        ret = upd(ret, get(i, j + 1, 3 - who_moves), who_moves);

    ret = upd_sum(ret, a[i][j], who_moves);

    return ret;
}

int main()
{
    for(int i = 0; i < N; i++)
        for(int j = 0; j < N; j++)
            cin >> a[i][j];

    cout << get(N - 1, 0, 1) << endl;
    return 0;
}
