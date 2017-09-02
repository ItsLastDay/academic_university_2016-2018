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

const int MAXN = 10100;

unsigned numbers[MAXN];
int sz[MAXN];

unsigned get_number()
{
    unsigned res = 0;

    int n;
    cin >> n;
    for(int i = 0; i < n; i++) {
        int cur;
        cin >> cur;
        res |= (1u << cur);
    }

    return res;
}

int main()
{
    cin.sync_with_stdio(false);

    int n;
    cin >> n;
    for(int i = 0; i < n; i++) {
        numbers[i] = get_number();
        sz[i] = __builtin_popcount(numbers[i]);
    }

    for(int i = 0; i < n; i++) {
        int worst_idx = i;
        int worst_val = 100;
        for(int j = 0; j < n; j++) {
            if(i == j)
                continue;
            int cur_val = sz[i] + sz[j] - 2 * __builtin_popcount(numbers[i] & numbers[j]);
            if(cur_val < worst_val) {
                worst_val = cur_val;
                worst_idx = j;
            }
        }

        cout << worst_idx + 1 << " " << worst_val << "\n";
    }
    return 0;
}
