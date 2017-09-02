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
const int MAXC = 1e9;

using rect_t = pair< pair<int, int>, pair<int, int> >;

pair<int, int> int_x(pair<int, int> a, pair<int, int> b)
{
    if(max(a.first, b.first) > min(a.second, b.second))
        return make_pair(0, 0);
    return make_pair(max(a.first, b.first), min(a.second, b.second));
}

rect_t inter(rect_t a, rect_t b)
{
    auto xx = int_x(make_pair(a.first.first, a.second.first), 
            make_pair(b.first.first, b.second.first));
    auto yy = int_x(make_pair(a.first.second, a.second.second),
                make_pair(b.first.second, b.second.second));

    return make_pair(make_pair(xx.first, yy.first), make_pair(xx.second, yy.second));
}

int main()
{
    cin.sync_with_stdio(false);
    cin.tie(NULL);
    int n;

    cin >> n;
    pair< pair<int, int>, pair<int, int> > cur =
        { {-MAXC, -MAXC}, {MAXC, MAXC} };
    for(int i = 0; i < n; i++)
    {
        int x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        cur = inter(cur, make_pair(make_pair(x1, y1), make_pair(x2, y2)));
    }

    long long res = cur.second.first - cur.first.first;
    res *= cur.second.second - cur.first.second;
    cout << res << endl;
    return 0;
}
