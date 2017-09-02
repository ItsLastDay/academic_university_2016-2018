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

const int MAXN = 505;
const pair<int, int> INF = make_pair(MAXN * MAXN, 0);

short field[MAXN][MAXN];

const int dx[8] = {1, 1, 1, 0, 0, -1, -1, -1};
const int dy[8] = {-1, 0, 1, -1, 1, -1, 0, 1};

pair<int, int> dist[MAXN][MAXN];
int n, m;

int main()
{   
    cin.sync_with_stdio(false);
    int start_x, start_y, end_x, end_y;
    cin >> n >> m;
    cin >> start_x >> start_y;
    cin >> end_x >> end_y;

    for (int i = 0; i < n; i++)
    {
        string s;
        cin >> s;
        for (int j = 0; j < m; j++)
        {
            field[i + 1][j + 1] = s[j] - '0';
        }
    }

    for (int i = 0; i <= n; i++)
        for (int j = 0; j <= m; j++)
            dist[i][j] = INF;

    dist[start_x][start_y] = make_pair(0, 1);

    set< pair< pair<int, int>, pair<int, int>> > q;
    q.insert(make_pair(dist[start_x][start_y], make_pair(start_x, start_y)));
    while (not q.empty())
    {
        auto cur = *q.begin();
        q.erase(q.begin());

        int i = cur.second.first;
        int j = cur.second.second;
        for (int k = 0; k < 8; k++)
        {
            int ni = i + dx[k];
            int nj = j + dy[k];
            if (ni < 1 or ni > n or nj < 1 or nj > m)
                continue;
            if (field[ni][nj] == 0)
                continue;

            int add = field[i][j] != field[ni][nj];
            auto new_dist = make_pair(dist[i][j].first + add, dist[i][j].second + 1); 
            if (new_dist < dist[ni][nj])
            {
                q.erase(make_pair(dist[ni][nj], make_pair(ni, nj)));
                dist[ni][nj] = new_dist;
                q.insert(make_pair(dist[ni][nj], make_pair(ni, nj)));
            }
        }
    }

    auto ans = dist[end_x][end_y];
    if (ans == INF)
        cout << "0 0" << endl;
    else
        cout << ans.second << " " << ans.first << endl;
    return 0;
}
