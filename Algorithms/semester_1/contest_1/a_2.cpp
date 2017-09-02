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


void solve(int n)
{
    for(int a = 1; a * a <= n; a++)
    {
        for(int b = a; b * b + a * a <= n; b++)
        {
            if(b * b + a * a == n)
                cout << "\t" << a << " " << b << endl;
        }
    }
}

int main()
{
    cin.sync_with_stdio(false);
    int low = 1000000;
    for(int i = low; i <= low + 100; i++)
    {
        cout << i << ":" << endl;
        solve(i);
    }
    return 0;
}
