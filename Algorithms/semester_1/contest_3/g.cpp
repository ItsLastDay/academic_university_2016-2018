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

const int MAXN = 2020;
const long long INF = -MAXN * 1e8 - 400;

long long dist[MAXN];
vector< tuple<int, int, int> > edges;

int main()
{
    cin.sync_with_stdio(false);
    int n, m;
    cin >> n >> m;
    for (int i = 0; i < m; i++)
    {
        int fr, to, weight;
        cin >> fr >> to >> weight;
        edges.push_back(make_tuple(fr, to, weight));
    }

    for (int i = 2; i <= n; i++)
        dist[i] = INF;

    bool last_change = false;
    for (int i = 0; i < n + 1; i++)
    {
        last_change = false;
        for (const auto &u: edges)
        {
            int fr, to, weight;
            tie(fr, to, weight) = u;
            if (dist[fr] == INF)
                continue;
            if (dist[to] < dist[fr] + weight)
            {
                last_change |= to == n;
                dist[to] = dist[fr] + weight;
            }
        }
    }

    if (last_change)
        cout << ":)" << endl;
    else if (dist[n] == INF)
        cout << ":(" << endl;
    else
        cout << dist[n] << endl;
    return 0;
}
