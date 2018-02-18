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
edge_t all_edges[555 * 555];
vector<edge_t> edges_residual[555 * 2];

void build_residual()
{
    for (int i = 0; i <= n; i++)
        edges_residual[i].clear();

    for (int i = 1; i <= m; i++)
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
            {
                get<3>(all_edges[-u]) -= min_cap;
            }
            else
            {
                get<3>(all_edges[u]) += min_cap;
            }
        }

        max_flow += min_cap;
    }

    return dist[to] != 0;
}

int tend[555];
pair<int, int> pnt_end[555];

int main()
{   
    cin.sync_with_stdio(false);
    cin.tie(NULL);

    int num_tr;
    cin >> num_tr;

    n = 2 * num_tr + 2;
    m = 0;

    int source = 0;
    int sink = 2 * num_tr + 2;

    for (int i = 1; i <= num_tr; i++)
    {
        int hh, mm, xf, yf, xt, yt;
        char tmp;
        cin >> hh >> tmp >> mm >> xf >> yf >> xt >> yt;
        int tm = 60 * hh + mm; 

        tend[i] = tm + abs(xf - xt) + abs(yf - yt);
        pnt_end[i] = make_pair(xt, yt);
        for (int j = 1; j < i; j++)
        {
            if (tend[j] + abs(pnt_end[j].first - xf) + abs(pnt_end[j].second - yf)
                    < tm)
            {
                all_edges[++m] = make_tuple(2 * j, 2 * i + 1, 1, 0);
            }
        }

        all_edges[++m] = make_tuple(source, 2 * i, 1, 0);
        all_edges[++m] = make_tuple(2 * i + 1, sink, 1, 0);
        all_edges[++m] = make_tuple(2 * i + 1, 2 * i, 555, 0);
    }


    long long answer = 0;
    build_residual();
    while (find_path(source, sink, answer))
    {
        build_residual();
    }
//    DBG(answer);

    cout << num_tr - answer << endl;
    return 0;
}
