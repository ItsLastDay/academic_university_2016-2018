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

const int MAXN = 2 * 1010;

vector<int> edges[MAXN];
string colors;
int n, m;
int vis[MAXN];

int cmps = 0;
int compnum[MAXN];
char avail_in_cmp[MAXN][2];

char color_of_component[MAXN];
char buf_color_of_component[MAXN];

vector< tuple<int, char, char> > restrict_edge[MAXN];

bool dfs_color(int v, char col)
{
    if (color_of_component[v])
    {
        return color_of_component[v] == col;
    }

    color_of_component[v] = col;
    for (const auto &u: restrict_edge[v])
    {
        int to;
        char iff, then;
        tie(to, iff, then) = u;
        if (iff != col)
            continue;
        bool res = dfs_color(to, then);
        if (not res)
        {
            return false;
        }
    }
    return true;
}


void no()
{
    cout << "Impossible" << endl;
    exit(0);
}

void dfs_split(int v, int parity)
{
    if (vis[v])
        return;
    vis[v] = true;
    compnum[v] = cmps - parity;

    for (const auto &u: edges[v])
    {
        if (colors[u] != colors[v])
            continue;
        if (compnum[u] == compnum[v])
        {
            no();
        }
        dfs_split(u, 1 - parity);
    }
}


void add_restricting_edge(int v, int u, char iff, char then)
{
    restrict_edge[v].push_back(make_tuple(u, iff, then));
}


int main()
{   
    cin.sync_with_stdio(false);
    cin >> n >> m;
    cin >> colors;
    colors = "0" + colors;
    for (int i = 0; i < m; i++)
    {
        int fr, to;
        cin >> fr >> to;
        edges[fr].push_back(to);
        edges[to].push_back(fr);
    }

    const vector<char> avail = {'R', 'G', 'B'};

    for (int i = 1; i <= n; i++)
    {
        if (not vis[i])
        {
            cmps++;
            cmps++;
            int ttt = 0;
            for (const auto& col: avail)
            {
                if (col == colors[i])
                    continue;
                avail_in_cmp[cmps - 1][ttt] = col;
                avail_in_cmp[cmps][ttt] = col;
                ++ttt;
            }
            dfs_split(i, 0);
        }
    }

    char can[3];
    int ttt; 
    for (int v = 1; v <= n; v++)
    {
        for (const auto &u: edges[v])
        {
            int num_v = compnum[v];
            int num_u = compnum[u];
            for (const auto& cu: avail)
            {
                if (cu == colors[u])
                    continue;
                ttt = 0;
                for (const auto& cv: avail)
                {
                    if (cv == colors[v] or cv == cu)
                        continue;
                    can[ttt++] = cv;
                }
                if (ttt == 1)
                {
                    add_restricting_edge(num_u, num_v, cu, can[0]);
                }
            }
        }
    }

    //cout << cmps << endl;
    for (int i = 1; i <= cmps; i++)
    {
        sort(restrict_edge[i].begin(), restrict_edge[i].end());
        restrict_edge[i].resize(unique(restrict_edge[i].begin(), restrict_edge[i].end()) - restrict_edge[i].begin());
        /*
        for (const auto &u: restrict_edge[i])
        {
            int to;
            char iff, then;
            tie(to, iff, then) = u;
            cout << i << " " << to << " " << iff << " " << then << endl;
        }
        */
    }

    for (int i = 1; i <= cmps; i++)
    {
        if (color_of_component[i] == 0)
        {
            memcpy(buf_color_of_component, color_of_component, sizeof(color_of_component));
            bool res = dfs_color(i, avail_in_cmp[i][0]);
            if (not res)
            {
                memcpy(color_of_component, buf_color_of_component, sizeof(color_of_component));
                res = dfs_color(i, avail_in_cmp[i][1]);
                if (not res)
                    no();
            }
        }
    }

    for (int i = 1; i <= n; i++)
        colors[i] = color_of_component[compnum[i]];
    cout << colors.substr(1) << endl;
    return 0;
}
