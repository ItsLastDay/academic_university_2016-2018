#include <bits/stdc++.h>
using namespace std;
using namespace std::chrono;
#define DBG(x) cout << #x << " = " << x << "\n";
/*
Compile:
    -Wall -Wextra -pedantic -std=c++14 -O2 -Wshadow -Wformat=2 -Wfloat-equal -Wconversion -Wlogical-op -Wcast-qual -Wcast-align -fwhole-program -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -lmcheck -fsanitize=address 

    -std=c++11 -O2
 
Check for memory usage:
    valgrind --tool=massif
*/

const int MAXN = 100500;
const int LOG = 20;


int jump_to[LOG][MAXN];
int min_edge[LOG][MAXN];
vector<int> edges[MAXN];
int timer = 0;
int depth[MAXN];
int tout[MAXN], tin[MAXN];


void dfs(int v)
{
    tin[v] = ++timer;
    for (const auto &u: edges[v])
        dfs(u);
    tout[v] = ++timer;
}

inline bool is_inside(int u, int v)
{
    // v in u's subtree
    return tin[u] <= tin[v] and tout[v] <= tout[u];
}


int get_ans(int fr, int num_jmp)
{
    int answer = 1e6 + 123;
    for (int i = LOG - 1; i >= 0; i--)
    {
        int cur = 1 << i;
        if (num_jmp < cur)
            continue;
        answer = min(answer, min_edge[i][fr]);
        num_jmp -= cur;
        fr = jump_to[i][fr];
    }

    return answer;
}


int get_jumps(int fr, int to)
{
    // jump from `to` to go to lca.
    if (is_inside(to, fr))
        return 0;

    int num_jumps = 0;
    for (int i = LOG - 1; i >= 0; i--)
    {
        int cur = 1 << i;
        if (not is_inside(jump_to[i][to], fr))
        {
            to = jump_to[i][to];
            num_jumps += cur;
        }
    }

    return num_jumps + 1;
}

int main()
{   
    cin.sync_with_stdio(false);
    cin.tie(NULL);

    for (int j = 0; j < LOG; j++)
        for (int i = 0; i < MAXN; i++) 
        {
            min_edge[j][i] = 1e6 + 13;
            jump_to[j][i] = 0;
        }

    jump_to[0][1] = 1;
    int n;
    cin >> n;
    for (int i = 2; i <= n; i++)
    {
        int par, cost;
        cin >> par >> cost;
        depth[i] = depth[par] + 1;

        edges[par].push_back(i);
        jump_to[0][i] = par;
        min_edge[0][i] = cost;
    }

    for (int j = 1; j < LOG; j++)
        for (int i = 1; i <= n; i++)
        {
            jump_to[j][i] = jump_to[j - 1][jump_to[j - 1][i]];
            min_edge[j][i] = min(min_edge[j - 1][i], min_edge[j - 1][jump_to[j - 1][i]]);
        }

    dfs(1);


    int q;
    cin >> q;
    for (int i = 0; i < q; i++)
    {
        int fr, to;
        cin >> fr >> to;
        int num_jumps_from_to = get_jumps(fr, to);
        int num_jumps_from_fr = get_jumps(to, fr);

        int mn = get_ans(fr, num_jumps_from_fr);
        mn = min(mn, get_ans(to, num_jumps_from_to));
        cout << mn << "\n";
    }
    return 0;
}
