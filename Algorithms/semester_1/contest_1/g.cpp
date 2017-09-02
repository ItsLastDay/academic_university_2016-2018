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

const int MAXN = 100500;

int n;
string names[MAXN];

int par[MAXN];
int max_id[MAXN], min_id[MAXN];

void init_dsu()
{
    for(int i = 1; i <= n; i++) 
    {
        par[i] = i;
        max_id[i] = i;
        min_id[i] = i;
    }
}

int get_par(int x)
{
    if(x == par[x])
        return x;
    return par[x] = get_par(par[x]);
}

void merge(int x, int y)
{
    x = get_par(x);
    y = get_par(y);
    assert(x != y);

    min_id[y] = min(min_id[y], min_id[x]);
    max_id[y] = max(max_id[y], max_id[x]);
    par[x] = y;
}

pair<string, string> solve(int pos)
{
    int xx = pos + 1;
    int yy = pos - 1;
    if(yy == 0)
        yy = n;
    if(xx == n + 1)
        xx = 1;

    xx = get_par(xx);
    yy = get_par(yy);

    cout << pos << " " << max_id[yy] << " " << min_id[xx] << endl;

    auto res = make_pair(names[max_id[yy]], names[min_id[xx]]);

    merge(xx, pos);
    merge(xx, yy);

    return res;
}

int main()
{
    cin.sync_with_stdio(false);
    cin.tie(NULL);
    cin >> n;
    for(int i = 1; i <= n; i++)
        cin >> names[i];
    init_dsu();

    for(int i = 0; i < n - 3; i++)
    {
        int x;
        cin >> x;
        auto res = solve(x);
        cout << res.first << " " << res.second << "\n";

        for(int j = 1; j <= n; j++) {
            int t = get_par(j);
            cout << t << " " << min_id[t] << " " << max_id[t] << endl;
        }
        cout << endl;
    }
    return 0;
}
