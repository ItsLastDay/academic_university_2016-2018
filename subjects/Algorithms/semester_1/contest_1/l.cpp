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

vector< tuple<int, int, int> > order;

int main()
{
    int n, k;
    cin.sync_with_stdio(false);
    cin >> n >> k;

    for(int i = 0; i < n; i++)
    {
        int v, w;
        cin >> v >> w;
        order.push_back(make_tuple(v, w, i));
    }

    long long best_l = 0, best_h = 0;

    auto sort_func = [&best_l, &best_h]
        (tuple<int, int, int> const& x, tuple<int, int, int> const& y)
    {
        long long aa = static_cast<long long>(get<0>(x) + best_h) * 
            (get<1>(y) + best_l);
        long long bb = static_cast<long long>(get<1>(x) + best_l) *
            (get<0>(y) + best_h);
        return aa > bb;
    };

    for (int i = 0; i < k; i++) 
    {
        auto it = min_element(order.begin(), order.end(), sort_func);
        auto qq = *it;
        order.erase(it);
        best_h += get<0>(qq);
        best_l += get<1>(qq);
    }

    cout << best_l << " " << best_h << endl;
    return 0;
}
