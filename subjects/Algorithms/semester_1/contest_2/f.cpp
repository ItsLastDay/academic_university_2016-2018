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

const int MAXN = 1e6 + 123;
const int LOG = 20 + 1;

int table[MAXN][LOG];
int depth[MAXN];

int jump_to(int u, int steps)
{
    for (int j = LOG - 1; j >= 0; j--)
    {
        int cur = 1 << j;
        if (steps >= cur)
        {
            steps -= cur;
            u = table[u][j];
        }
    }

    return u;
}

int lca(int u, int v)
{
    if (depth[u] < depth[v])
        swap(u, v);
    u = jump_to(u, -(depth[v] - depth[u]));
    if (u == v)
        return v;

    for (int j = LOG - 1; j >= 0; j--)
    {
        int par_one = table[u][j];
        int par_two = table[v][j];
        if (par_one == par_two)
            continue;
        u = par_one;
        v = par_two;
    }

    return jump_to(u, 1);
}

int main()
{   
    cin.sync_with_stdio(false);
    cin.tie(NULL);
    int n;
    int cur_vertex = 1;
    for (int i = 0; i < LOG; i++)
    {
        table[1][i] = 1;
    }

    cin >> n;
    for (int i = 0; i < n; i++)
    {
        string cmd;
        cin >> cmd;
        if (cmd == "add")
        {
            int par;
            cin >> par;
            ++cur_vertex;
            table[cur_vertex][0] = par;
            for (int j = 1; j < LOG; j++)
            {
                table[cur_vertex][j] = table[table[cur_vertex][j - 1]][j - 1];
            }
            depth[cur_vertex] = depth[par] + 1;
        }
        else
        {
            int u, v;
            cin >> u >> v;
            cout << lca(u, v) << "\n";
        }
    }
    return 0;
}
