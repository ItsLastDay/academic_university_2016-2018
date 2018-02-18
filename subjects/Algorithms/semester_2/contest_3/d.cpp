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

const int MAXN = 1005000;

int p[MAXN], cnt[MAXN];

int main()
{   
    cin.sync_with_stdio(false);
    cin.tie(NULL);

    string s;
    cin >> s;

    int n = s.size();


    int j = 0;
    for (int i = 1; i < n; i++)
    {
        while (j > 0 and s[j] != s[i])
            j = p[j - 1];
        if (s[j] == s[i])
            j++;
        p[i] = j;
    }

    int mx = 0;
    int sub = 1;
    for (int i = 1; i < n; i++)
    {
        cnt[i] = p[i] == 0 ? 0 : 1 + cnt[p[i] - 1];
        if (cnt[i] > mx)
        {
            mx = cnt[i];
            sub = i + 1;
        }
    } 

    cout << s.substr(0, sub) << endl;
    return 0;
}
