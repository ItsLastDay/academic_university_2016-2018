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

const int MAXN = 10100;

vector<int> edges[MAXN];
vector<int> rev[MAXN];
vector<int> topsort;

int cmps = 0;
int vis[MAXN];
set< pair<int, int> > cmp_edges;

void dfs(int v)
{
    if (vis[v])
        return;
    vis[v] = true;
    for (const auto &u: edges[v])
        dfs(u);
    topsort.push_back(v);
}


void dfs2(int v)
{
    if (vis[v])
    {
        if (vis[v] != cmps)
            cmp_edges.insert(make_pair(vis[v], cmps));
        return;
    }
    vis[v] = cmps;
    for (const auto &u: rev[v])
        dfs2(u);
}


int main()
{   
    cin.sync_with_stdio(false);
    int n, m;
    cin >> n >> m;
    for (int i = 0; i < m; i++)
    {
        int fr, to;
        cin >> fr >> to;
        edges[fr].push_back(to);
        rev[to].push_back(fr);
    }


    for (int i = 1; i <= n; i++)
        dfs(i);    

    fill(vis, vis + n + 1, 0);

    reverse(topsort.begin(), topsort.end());
    for (const auto &u: topsort)
    {
        if (vis[u] != 0)
            continue;
        cmps++;
        dfs2(u);
    }

    cout << cmp_edges.size() << endl;
    return 0;
}
