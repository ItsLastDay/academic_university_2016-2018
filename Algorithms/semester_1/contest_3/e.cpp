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

const int MAXN = 5050;
const int INF = 1e5 * MAXN + 123;

vector< pair<int, int> > edges[MAXN];
int dist[MAXN];
int pr[MAXN];

int main()
{   
    cin.sync_with_stdio(false);
    int n, m;
    cin >> n >> m;
    int st, fin;
    cin >> st >> fin;

    for (int i = 1; i <= n; i++)
        dist[i] = INF;
    dist[st] = 0;

    for (int i = 0; i < m; i++)
    {
        int fr, to, weight;
        cin >> fr >> to >> weight;
        edges[fr].push_back(make_pair(to, weight));
        edges[to].push_back(make_pair(fr, weight));
    }

    set< pair<int, int> > q;
    q.insert(make_pair(dist[st], st));
    while (not q.empty())
    {
        auto cur = *q.begin();
        q.erase(q.begin());

        int cur_dst = cur.first;
        for (const auto &u: edges[cur.second])
        {
            int new_to = u.first;
            int new_w = u.second + cur_dst;
            if (new_w < dist[new_to])
            {
                q.erase(make_pair(dist[new_to], new_to));
                pr[new_to] = cur.second;
                dist[new_to] = new_w;
                q.insert(make_pair(dist[new_to], new_to));
            }
        }
    }

    if (dist[fin] == INF)
    {
        cout << -1 << endl;
    }
    else
    {
        cout << dist[fin] << "\n";
        vector<int> answer;
        while (fin != st)
        {
            answer.push_back(fin);
            fin = pr[fin];
        }
        answer.push_back(st);
        reverse(answer.begin(), answer.end());
        for (const auto &u: answer)
            cout << u << " ";
    }
    return 0;
}

