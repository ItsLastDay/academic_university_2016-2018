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

const int MAXN = 110;

int par[MAXN];

int get_par(int v)
{
    if (v == par[v])
        return v;
    return par[v] = get_par(par[v]);
}

void merge(int v, int u)
{
    v = get_par(v);
    u = get_par(u);
    par[u] = v;
}

int main()
{   
    cin.sync_with_stdio(false);
    int n, m;
    cin >> n >> m;
    for (int i = 1; i <= n; i++)
        par[i] = i;

    for (int i = 0; i < m; i++)
    {
        int fr, to;
        cin >> fr >> to;
        if (get_par(fr) != get_par(to))
        {
            cout << fr << " " << to << "\n";
            merge(fr, to);
        }
    }
    return 0;
}
