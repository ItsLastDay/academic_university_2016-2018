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

int answer[MAXN];

int main()
{
    int n, m;
    cin.sync_with_stdio(false);
    cin.tie(NULL);
    cin >> n >> m;

    vector< tuple<int, int, int> > events;

    for(int i = 0; i < n; i++)
    {
        int a, b;
        cin >> a >> b;
        if(a > b)
            swap(a, b);
        events.push_back(make_tuple(a, -1, 0));
        events.push_back(make_tuple(b, 1, 0));
    }

    for(int i = 0; i < m; i++) 
    {
        int a;
        cin >> a;
        events.push_back(make_tuple(a, 0, i));
    }

    sort(events.begin(), events.end());

    int cur_open = 0;
    for(const auto &u: events)
    {
        if(get<1>(u) == -1)
            cur_open++;
        else if(get<1>(u) == 1)
            cur_open--;
        else 
        {
            answer[get<2>(u)] = cur_open;
        }
    }

    for(int i = 0; i < m; i++)
        cout << answer[i] << " ";
    return 0;
}
