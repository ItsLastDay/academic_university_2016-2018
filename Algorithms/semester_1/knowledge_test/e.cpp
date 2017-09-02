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


int cur_division[44];
int cur_idx = 0;

void rec(int n, int last)
{
    if(n == 0) {
        for(int i = 0; i < cur_idx; i++)
            cout << cur_division[i] << " ";
        cout << "\n";
        return;
    }

    for(int i = 1; i <= min(last, n); i++) {
        cur_division[cur_idx++] = i;
        rec(n - i, i);
        cur_idx--;
    }
}

int main()
{
    cin.sync_with_stdio(false);
    int n;
    cin >> n;

    rec(n, n + 1);
    return 0;
}
