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

const int MAXN = 20050;

int par[MAXN];
int sz[MAXN];
int x[MAXN], y[MAXN];

int get(int v)
{
    if (v == par[v])
        return v;
    return par[v] = get(par[v]);
}

void merge(int a, int b)
{
    a = get(a);
    b = get(b);
    if (sz[a] < sz[b])
        swap(a, b);

    if (a != b)
    {
        sz[a] += sz[b];
        par[b] = a;
    } 
}


int main()
{   
    cin.sync_with_stdio(false);
    cin.tie(NULL);

    vector<tuple<int, int, int>> edges;

    for (int i = 0; i < MAXN; i++)
    {
        par[i] = i;
        sz[i] = 1;
    }

    int n, m;
    cin >> n >> m;
    edges.reserve(m);
    for (int i = 0; i < m; i++)
    {
        int fr, to, cost;
        cin >> fr >> to >> cost;
        edges.push_back(make_tuple(cost, fr, to));
    }

    sort(edges.begin(), edges.end());
    int answer = 0;

    for (const auto &u: edges)
    {
        int cost;
        int fr, to;
        tie(cost, fr, to) = u;

        if (get(fr) != get(to))
        {
            answer += cost;
            merge(fr, to);
        }
    }

    cout << answer << endl;

    return 0;
}
