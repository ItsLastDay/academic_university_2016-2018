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

const int MAXN = 20200;

vector<int> edges[MAXN];
int comp[MAXN];
int comps_so_far = 0;

void dfs(int v)
{
    if(comp[v] != 0)
        return;
    comp[v] = comps_so_far;

    for(const auto &u: edges[v])
        dfs(u);
}

int main()
{
    cin.sync_with_stdio(false);
    int n, m;
    cin >> n >> m;

    for(int i = 0; i < m; i++) {
        int fr, to;
        cin >> fr >> to;
        edges[fr].push_back(to);
        edges[to].push_back(fr);
    }

    for(int i = 1; i <= n; i++) {
        if(!comp[i])
            comps_so_far++;
        dfs(i);
    }

    cout << comps_so_far << "\n";
    for(int i = 1; i <= n; i++)
        cout << comp[i] << " ";
    return 0;
}
