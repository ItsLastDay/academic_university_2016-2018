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

const int MAXN = 50500;
const int LOG = 17;

vector<int> edges[MAXN];
int sparse_table[LOG][MAXN];

int timer = 1;
int tin[MAXN], tout[MAXN];

void dfs(int v, int pr=-1)
{
    tin[v] = ++timer;
    for(const auto &u: edges[v])
        if(u != pr)
            dfs(u, v);

    tout[v] = ++timer;
}

inline bool is_in_subtree(int u, int v)
{
    // v in u's subtree
    return tin[v] >= tin[u] and tout[v] <= tout[u];
}

int get_lca(int u, int v)
{
    if(is_in_subtree(u, v))
        return u;
    if(is_in_subtree(v, u))
        return v;

    for(int i = LOG - 1; i >= 0; i--) {
        int to = sparse_table[i][v];
        if(not is_in_subtree(to, u))
            v = to;
    }

    return sparse_table[0][v];
}

int main()
{
    cin.sync_with_stdio(false);
    int n;
    cin >> n;

    sparse_table[0][1] = 1;
    for(int i = 2; i <= n; i++) {
        int pr;
        cin >> pr;
        sparse_table[0][i] = pr;
        edges[i].push_back(pr);
        edges[pr].push_back(i);
    }

    for(int i = 1; i < LOG; i++) {
        for(int j = 1; j <= n; j++) {
            sparse_table[i][j] = sparse_table[i - 1][sparse_table[i - 1][j]];
        }
    }

    dfs(1);

    int q;
    cin >> q;
    for(int i = 0; i < q; i++) {
        int fr, to;
        cin >> fr >> to;
        cout << get_lca(fr, to) << "\n";
    }

    return 0;
}
