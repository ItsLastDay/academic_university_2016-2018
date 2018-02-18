#include <bits/stdc++.h>
using namespace std;
using namespace std::chrono;
#define DBG(x) cout << #x << " = " << (x) << endl;
/*
Compile:
    -Wall -Wextra -pedantic -std=c++14 -O2 -Wshadow -Wformat=2 -Wfloat-equal -Wconversion -Wlogical-op -Wcast-qual -Wcast-align -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -D_FORTIFY_SOURCE=2 -fsanitize=address -fsanitize=undefined -fno-sanitize-recover -fstack-protector 

    -std=c++14 -O2
 
Check for memory usage:
    valgrind --tool=massif
*/

const int MAXN = 100500;

string s[MAXN];

int main()
{   
    cin.sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;
    for (int i = 0; i < n; i++)
    {
        cin >> s[i];
    }

    int answer = 0;
    bool x = false;

    for (int i = n - 1; i >= 0; i--)
    {
        bool now_good = s[i] == "W";
        now_good ^= x;

        if (not now_good)
        {
            answer++;
            x ^= 1;
        }
    }

    cout << answer << endl;
    return 0;
}
