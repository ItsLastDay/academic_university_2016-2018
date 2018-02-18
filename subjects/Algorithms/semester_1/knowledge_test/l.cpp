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

const int MAXM = 10100;
const int MAXN = 2020;

long long dist[MAXN];
int fr[MAXM], to[MAXM], cost[MAXM];

int main()
{
    cin.sync_with_stdio(false);

    int n, m;
    cin >> n >> m;
    fill(dist, dist + n, 1e18);
    dist[1] = 0;

    for(int i = 0; i < m; i++) {
        cin >> fr[i] >> to[i] >> cost[i];
    }

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            
        }
    }
    return 0;
}
