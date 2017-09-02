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

int go(int fr, int to, unordered_map<int, int>& pos)
{
    int fr_pos = pos[fr];
    if (fr_pos == 0)
        fr_pos = fr;

    int to_pos = pos[to];
    if (to_pos == 0)
        to_pos = to;

    int res = abs(to_pos - fr_pos);

    pos[fr] = to_pos;
    pos[to] = fr_pos;

    return res;
}

int main()
{   
    cin.sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;

    unordered_map<int, int> pos;

    for (int i = 0; i < n; i++)
    {
        int fr, to;
        cin >> fr >> to;

        cout << go(fr, to, pos) << "\n";
    }
    return 0;
}
