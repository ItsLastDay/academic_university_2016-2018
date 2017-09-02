#include <bits/stdc++.h>
using namespace std;
using namespace std::chrono;
#define DBG(x) cout << #x << " = " << x << "\n";
/*
Compile:
    -Wall -Wextra -pedantic -std=c++14 -O2 -Wshadow -Wformat=2 -Wfloat-equal -Wconversion -Wlogical-op -Wcast-qual -Wcast-align -fwhole-program -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -lmcheck -fsanitize=address 

    -std=c++11 -O2
 
Check for memory usage:
    valgrind --tool=massif
*/

const int MAXN = 5050;

long double x[MAXN], y[MAXN];
long double cur_dist[MAXN];
bool was[MAXN];

long double get_dist(int i, int j)
{
    long double cost = (x[i] - x[j]) * (x[i] - x[j]);
    cost += (y[i] - y[j]) * (y[i] - y[j]);
    cost = sqrt(cost);
    return cost;
}

int main()
{   
    cin.sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;
    for (int i = 0; i < n; i++)
    {
        cin >> x[i] >> y[i];
    }

    for (int i = 0; i < n; i++)
        cur_dist[i] = 1e10;
    cur_dist[0] = 0;

    long double answer = 0;

    for (int i = 0; i < n; i++) 
    {
        int min_idx = 0;
        for (int j = 0; j < n; j++) 
        {
            if (not was[j] and (was[min_idx] or cur_dist[min_idx] > cur_dist[j]))
            {
                min_idx = j;
            }
        }

        was[min_idx] = true;
        answer += cur_dist[min_idx];
        for (int j = 0; j < n; j++) 
        {
            cur_dist[j] = min(cur_dist[j], get_dist(min_idx, j));
        }
    }

    cout << setprecision(20) << answer << endl;

    return 0;
}
