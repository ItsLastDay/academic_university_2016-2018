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

int main()
{   
    cin.sync_with_stdio(false);
    int n, m;
    cin >> n >> m;

    for (int i = 1; i < n; i++)
    {
        cout << i << " " << i + 1 << " 0\n";
        m--;
    }

    for (int i = 1; m and i < n; i++)
    {
        int w = 1e5 - i;
        for (int j = n; m and j > i + 1; j--, m--)
        {
            cout << i << " " << j << " " << w << endl;
        }
    }
    return 0;
}
