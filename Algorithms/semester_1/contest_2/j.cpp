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

const int MAXN = 1e6 + 123;

int high[MAXN], low[MAXN];

int min_val[MAXN];

int main()
{   
    cin.sync_with_stdio(false);
    int n;
    cin >> n;
    for (int i = 1; i <= n; i++)
    {
        cin >> high[i];
    }

    for (int i = 1; i <= n; i++)
    {
        cin >> low[i];
    }

    map<int, int> occ;
    for (int i = 1; i <= n; i++)
    {
        occ[high[i]] = i;
        high[i] = i;
    }
    for (int i = 1; i <= n; i++)
        low[i] = occ[low[i]];


    for (int i = 1; i <= n + 2; i++)
        min_val[i] = n + 123;

    min_val[0] = 0;

    for (int i = 1; i <= n; i++)
    {
        int x = low[i];
        int le = 0, rg = i + 1;
        while (rg - le > 1)
        {
            int mid = (le + rg) / 2;
            if (min_val[mid] < x)
                le = mid;
            else
                rg = mid;
        }
        min_val[le + 1] = x;
    }

    for (int i = n; i >= 0; i--)
        if (min_val[i] != n + 123)
        {
            cout << i << endl;
            break;
        }

    return 0;
}
