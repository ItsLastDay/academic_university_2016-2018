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

const int MAXN = 55;

int n, m;
bool is_red[MAXN];
string mat[MAXN];

int vis[MAXN];

void dfs(int v, int& vert_cnt, int& edge_cnt, bool& seen_red)
{
    if (vis[v])
        return;
    vis[v] = 1;
    ++vert_cnt;
    seen_red |= is_red[v];
    for (int i = 0; i < n; i++)
    {
        if (mat[v][i] == '1')
        {
            ++edge_cnt;
            dfs(i, vert_cnt, edge_cnt, seen_red);
        }
    }
}

int main()
{   
    cin.sync_with_stdio(false);
    cin >> n;
    for (int i = 0; i < n; i++)
        cin >> mat[i];

    cin >> m;
    for (int i = 0; i < m; i++)
    {
        int t;
        cin >> t;
        is_red[t] = true; 
    }

    int answer = 0;
    vector< pair<int, int> > red_comps, blue_comps;

    for (int i = 0; i < n; i++)
    {
        if (vis[i])
            continue;
        int vv = 0, ee = 0;
        bool seen = 0;
        dfs(i, vv, ee, seen);
        ee /= 2;
//        cout << vv <<  " vv " << ee << " " << seen << endl;
        if (seen)
        {
            red_comps.push_back(make_pair(vv, ee));
        }
        else
            blue_comps.push_back(make_pair(vv, ee));

    }

    for (const auto &u: red_comps)
    {
        int vert = u.first;
        int edge = u.second;
        answer += max(0, vert * (vert - 1) / 2 - edge);
    }

    for (const auto &u: blue_comps)
    {
        int vert = u.first;
        int edge = u.second;
        answer += max(0, vert * (vert - 1) / 2 - edge);
    }

    while (blue_comps.size() > 1)
    {
        int idx_max = max_element(blue_comps.begin(), blue_comps.end()) - blue_comps.begin();
        int idx_sec = idx_max == 0 ? 1 : 0;
        for (size_t i = 0; i < blue_comps.size(); i++)
        {
            if (i == (size_t)idx_max)
                continue;
            if (blue_comps[i] > blue_comps[idx_sec])
                idx_sec = i;
        }

        int v1 = blue_comps[idx_max].first;
        int v2 = blue_comps[idx_sec].first;
//        cout << v1 << " " << v2 << " " << answer << endl;
        answer += v1 * v2;
        vector <pair<int, int> > new_blue;
        for (int i = 0; i < (int)blue_comps.size(); i++)
        {
            if (i == idx_max or i == idx_sec)
                continue;
            new_blue.push_back(blue_comps[i]);
        }
        new_blue.push_back(make_pair(v1 + v2, 0));
        blue_comps = new_blue;
    }

    if (red_comps.size() > 0 and blue_comps.size() > 0)
    {
        int v1 = max_element(red_comps.begin(), red_comps.end())->first;
        int v2 = blue_comps[0].first;
        answer += v1 * v2;
    }

    cout << answer << endl;
    return 0;
}
