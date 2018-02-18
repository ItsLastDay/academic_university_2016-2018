#include <vector>
#include <string>
#include <iostream>
using namespace std;
#define DBG(x) cout << #x << " = " << x << "\n";
/*
Compile:
    -Wall -Wextra -pedantic -std=c++11 -O2 -Wshadow -Wformat=2 -Wfloat-equal -Wconversion -Wlogical-op -Wcast-qual -Wcast-align -fwhole-program -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -lmcheck -fsanitize=address 

    -std=c++11 -O2
 
Check for memory usage:
    valgrind --tool=massif
*/

const int MAXN = 100500;

string names[MAXN];
int nx[MAXN], pr[MAXN];

int main()
{
    cin.sync_with_stdio(false);
    cin.tie(NULL);
    int n;
    cin >> n;

    for(int i = 0; i < n; i++) {
        cin >> names[i];
    }

    for(int i = 0; i < n; i++) 
    {
        nx[i] = (i + 1) % n;
        pr[i] = (i - 1 + n) % n;
    }

    for(int i = 0; i < n - 3; i++)
    {
        int t;
        cin >> t;
        t--;
        int pos = t;
        cout << names[pr[pos]];
        cout << " " << names[nx[pos]] << "\n";

        nx[pr[pos]] = nx[pos];
        pr[nx[pos]] = pr[pos];
    }

    return 0;
}
