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

const int MAXN = 10000 + 123;
const int MAX_DAYS = 50000 + 123;

long long gold[MAXN];
string name[MAXN];
map<string, string> now_in;
map<string, int> name_to_idx;
map<string, int> city_to_win;
map<string, long long> cur_gold;

set< pair<long long, string>, std::greater< pair<long long, string> > > gold_sorted;

void add_gold(string cc, long long val)
{
    gold_sorted.erase(make_pair(cur_gold[cc], cc));
    cur_gold[cc] += val;
    gold_sorted.insert(make_pair(cur_gold[cc], cc));
}

void add_largest()
{
    auto it = gold_sorted.begin();
    auto itt = gold_sorted.begin();
    advance(itt, 1);
    if (itt != gold_sorted.end() and itt->first == it->first)
    {
        return;
    }
    city_to_win[it->second]++;
}

int main()
{   
    cin.sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;
    for (int i = 0; i < n; i++)
    {
        string cc;
        cin >> name[i] >> cc >> gold[i];
        name_to_idx[name[i]] = i;
        now_in[name[i]] = cc;
        add_gold(cc, gold[i]);
    }

    int m, k;
    cin >> m >> k;
    int now_day = 0;
    for (int i = 0; i < k; i++)
    {
        int q_day;
        string oli, city_to;
        cin >> q_day >> oli >> city_to;
        
        while (now_day < q_day) 
        {
            add_largest();
            now_day++;
        }

        auto gld = gold[name_to_idx[oli]];
        add_gold(now_in[oli], -gld);
        add_gold(city_to, gld);
        now_in[oli] = city_to;
    }

    while (now_day < m) 
    {
        add_largest();
        now_day++;
    }

    for (const auto &kv: city_to_win)
    {
        cout << kv.first << " " << kv.second << "\n";
    }
    return 0;
}
