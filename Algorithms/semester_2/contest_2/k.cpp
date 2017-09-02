#include <bits/stdc++.h>
using namespace std;
using namespace std::chrono;
#define DBG(x) cout << #x << " = " << (x) << endl;
/*
Compile:
    -Wall -Wextra -pedantic -std=c++14 -O2 -Wshadow -Wformat=2 -Wfloat-equal -Wconversion -Wlogical-op -Wcast-qual -Wcast-align -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -D_FORTIFY_SOURCE=2 -fsanitize=address -fsanitize=undefined -fno-sanitize-recover -fstack-protector 

    -std=c++14 -O2
 
Check for memory usage:
    valgrind --tool=massif
*/

// fr, to, cap, flow
using edge_t = tuple<int, int, long long, long long>;

int n, m;
edge_t all_edges[444 * 2];
vector<edge_t> edges_residual[111];

void build_residual()
{
    for (int i = 1; i <= n; i++)
        edges_residual[i].clear();

    for (int i = 1; i <= 2 * m; i++)
    {
        int fr;
        long long cur_flow;
        int to;
        long long cap;
        tie(fr, to, cap, cur_flow) = all_edges[i];

        if (cur_flow > 0)
        {
            edges_residual[to].emplace_back(fr, to, cur_flow, -i);
        }
        if (cur_flow < cap)
        {
            edges_residual[fr].emplace_back(to, fr, cap - cur_flow, i);
        }
    }
}

void find_cut(vector<int>& edges_cut, int start)
{
    vector<bool> visited(n + 1, false);
    visited[start] = true;
    queue<int> bfs;
    bfs.push(start);

    while (not bfs.empty())
    {
        int cur = bfs.front();
        bfs.pop();

        for (const auto &e: edges_residual[cur])
        {
            int nx = get<0>(e);
            if (not visited[nx])
            {
                visited[nx] = true;
                bfs.push(nx);
            }
        }
    }

    for (int i = 1; i <= 2 * m; i++)
    {
        int fr, to;
        tie(fr, to, ignore, ignore) = all_edges[i];
        if (visited[fr] ^ visited[to])
        {
            edges_cut.push_back(i <= m ? i : i - m);
        }
    }
}

bool find_path(int fr, int to, long long &max_flow)
{
    queue<int> bfs;
    bfs.push(fr);
    vector<edge_t> prev(n + 1);
    vector<int> dist(n + 1, 0);

    dist[fr] = 1;
    while (not bfs.empty())
    {
        auto cur = bfs.front();
        bfs.pop();

        for (const auto &e: edges_residual[cur])
        {
            int nx = get<0>(e);
            if (dist[nx] != 0)
                continue;
            dist[nx] = dist[cur] + 1;
            prev[nx] = e;
            bfs.push(nx);
        }
    }

    if (dist[to] != 0)
    {
        vector<int> edge_numbers;
        long long min_cap = 1e15;
        while (to != fr)
        {
            auto e = prev[to];

            min_cap = min(min_cap, get<2>(e));
            edge_numbers.push_back(get<3>(e));

            to = get<1>(e);
        }

        for (auto &u: edge_numbers)
        {
            if (u < 0)
                get<3>(all_edges[-u]) -= min_cap;
            else
                get<3>(all_edges[u]) += min_cap;
        }

        max_flow += min_cap;
    }

    return dist[to] != 0;
}

int main()
{   
    cin.sync_with_stdio(false);
    cin.tie(NULL);

    cin >> n >> m;
    for (int i = 1; i <= m; i++)
    {
        int fr, to, cap;
        cin >> fr >> to >> cap;
        all_edges[i] = make_tuple(fr, to, cap, 0);
        all_edges[m + i] = make_tuple(to, fr, cap, 0);
    }

    long long max_flow = 0;
    while (true)
    {
        build_residual();

        auto path = find_path(1, n, max_flow);
        if (not path)
            break;
    }

    vector<int> edges_cut;
    find_cut(edges_cut, 1);

    sort(edges_cut.begin(), edges_cut.end());
    edges_cut.resize(
            distance(edges_cut.begin(), unique(edges_cut.begin(), edges_cut.end())));
    cout << edges_cut.size() << " " << max_flow << "\n";
    for (auto& u: edges_cut)
    {
        cout << u << " ";
    }
    return 0;
}
