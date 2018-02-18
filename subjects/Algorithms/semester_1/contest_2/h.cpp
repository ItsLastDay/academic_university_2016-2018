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

const int MAXN = 1e7 + 2;
const short MOD = 10007;

short val[MAXN];


int main()
{   
    cin.sync_with_stdio(false);
    val[0] = 1;
    for (int i = 1; i < MAXN; i++)
    {
        val[i] = val[i - 1] * 2;
        while (val[i] >= MOD)
            val[i] -= MOD;
    }
    int n;
    cin >> n;

    short answer = 0;
    for (int i = n - 1; i >= 0; i -= 2)
    {
        answer += val[i];
        while (answer >= MOD)
            answer -= MOD;
    }

    cout << answer << endl;
    return 0;
}
