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

int LIM = 1e4;

int main()
{
    cin.sync_with_stdio(false);
    int a, b;
    cin >> a >> b;
    for(int x = -LIM; x <= LIM; x++) {
        int rhs = 1 - a * x;
        if(rhs % b)
            continue;
        int y = rhs / b;
        if(abs(y) <= LIM) {
            cout << x << " " << y;
            return 0;
        }
    }
    cout << "0 0";
    return 0;
}
