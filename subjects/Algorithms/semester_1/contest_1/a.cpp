

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

const int MAXN = 31623 + 234;

int sq[MAXN];

inline bool is_sq(int x)
{
    int r = static_cast<int>(sqrt(x));
    for(int j = -1; j <= 1; j++)
        if(r + j > 0 and (r + j) * (r + j) == x)
            return true;
    return false;
}

int solve(int n)
{
    int sols = 0;

    int nh = static_cast<int>(sqrt(n)) + 10;

    for(int b = nh; b >= 1; b--)
    {
        int bb = sq[b];
        int t = n - bb;
        if(t <= 0)
            continue;

        if(bb < t)
            break;
        if(is_sq(t)) {
            sols++;
            if(bb != t)
                sols++;
        }
    }

    return sols;
}

int main()
{
    cin.sync_with_stdio(false);
    cin.tie(NULL);
    for(int i = 1; i < MAXN; i++) {
        sq[i] = i * i;
    }
    int t = 1;
    int n;
    cin >> t;
    for(int i = 0; i < t; i++)
    {
        cin >> n;
        cout << solve(n) << "\n";
    }
    return 0;
}


