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

const int MAXN = 100010;

struct multiple_segment_tree
{
    multiple_segment_tree() {}

    multiple_segment_tree(vector<int> &numbers)
    {
        sort(numbers.begin(), numbers.end());
        sz = numbers.size();
        for (size_t i = 0; i < sz; i++)
        {
            number_to_idx[numbers[i]] = i;
        }

        sum.resize(4 * sz);
        sum.shrink_to_fit();
        cnt.resize(4 * sz);
        cnt.shrink_to_fit();

        heights.resize(sz);
        for (size_t i = 0; i < sz; i++)
            heights[i] = numbers[i];
    }

    void add(int number, int val)
    {
        deferred.push_back(make_pair(number_to_idx[number], val));
//        add(0, 0, sz - 1, number_to_idx[number], val);
    }

    long long query(long long len)
    {
        perform_deferred();
        long long present = cnt[0];
        long long left_idx, right_idx;

        if (len % 2 == present % 2)
        {
            if (present % 2 == 1)
            {
                long long mid = (present + 1) / 2;
                left_idx = mid - (len - 1) / 2;
                right_idx = mid + (len - 1) / 2;
            } 
            else
            {
                long long mid = present / 2;
                left_idx = mid - len / 2 + 1;
                right_idx = mid + len / 2;
            }
        }
        else 
        {
            if (present % 2 == 1)
            {
                long long mid = (present + 1) / 2;
                len--;
                left_idx = mid - (len - 1) / 2 - 1;
                right_idx = mid + (len - 1) / 2;
            }
            else
            {
                len--;
                long long mid = present / 2;
                left_idx = mid - len / 2;
                right_idx = mid + len / 2;
            }
        }

        return query(0, 0, sz - 1, left_idx, right_idx, 1, present);
    }

private:
    long long query(int v, int le, int rg, long long need_le, long long need_rg,
            long long now_le, long long now_rg)
    {
        if (need_le <= now_le and now_rg <= need_rg)
            return sum[v];
        if (need_le > now_rg or need_rg < now_le)
            return 0;
        if (le == rg)
        {
            long long fs = max(need_le, now_le);
            long long sc = min(now_rg, need_rg);
            return heights[le] * (sc - fs + 1);
        }
        long long size_left = cnt[2 * v + 1];
        
        auto x = query(v * 2 + 1, le, (le + rg) / 2, need_le, need_rg,
                now_le, now_le + size_left - 1);
        auto y = query(v * 2 + 2, (le + rg) / 2 + 1, rg, need_le, need_rg,
                now_le + size_left, now_rg);

        return x + y;
    }

    void add_all(int v, int le, int rg, int idx_le, int idx_rg)
    {
        if (idx_le > idx_rg)
            return;
//        cout << le << " " << rg << " " << idx_le << " " << idx_rg << endl;
        sum[v] += add_sum[idx_rg + 1] - add_sum[idx_le];
        cnt[v] += add_cnt[idx_rg + 1] - add_cnt[idx_le];
//        cout << sum[v] << " " << cnt[v] << endl;
        if (le == rg)
            return;
        int new_rg = idx_rg;
        while (new_rg >= idx_le and deferred[new_rg].first > (le + rg) / 2)
            new_rg--;
        add_all(v * 2 + 1, le, (le + rg) / 2, idx_le, new_rg);
        int new_le = idx_le;
        while(new_le <= idx_rg and deferred[new_le].first < (le + rg) / 2 + 1)
            new_le++;
        add_all(v * 2 + 2, (le + rg) / 2 + 1, rg, new_le, idx_rg);
    }

    void perform_deferred()
    {
        if (deferred.empty())
            return;
        sort(deferred.begin(), deferred.end());
        size_t n = deferred.size();
        size_t write_to = 0;
        for (size_t i = 0; i < n;)
        {
            size_t j = i;
            while (j < n and deferred[j].first == deferred[i].first)
                j++;
            long long combined_value = 0;
            for (size_t k = i; k < j; k++)
                combined_value += deferred[k].second;
//            cout << deferred[i].first << " " << combined_value << endl;
            deferred[write_to++] = make_pair(deferred[i].first, combined_value);
            i = j;
        }

        n = write_to;
        deferred.resize(n);
        add_sum = vector<long long>(n + 1, 0);
        add_cnt = vector<long long>(n + 1, 0);
        for (size_t i = 1; i <= n; i++)
        {
            add_cnt[i] = add_cnt[i - 1] + deferred[i - 1].second;
            add_sum[i] = add_sum[i - 1] + 
                heights[deferred[i - 1].first] * deferred[i - 1].second;
        }

        add_all(0, 0, sz - 1, 0, n - 1);

        deferred.clear();
    }

    size_t sz;
    unordered_map<int, int> number_to_idx;
    vector<long long> add_sum, add_cnt;
    vector< pair<int, int> > deferred;
    vector<long long> cnt;
    vector<long long> sum;
    vector<long long> heights;
};

vector<int> divisors[MAXN];
vector<int> multiples[MAXN];
multiple_segment_tree tree[MAXN];

vector<int> get_divisors(int n)
{
    vector<int> res;
    for (int i = 1; i * i <= n; i++)
    {
        if (n % i != 0)
            continue;
        res.push_back(i);
        if (i != n / i)
            res.push_back(n / i);
    }
    return res;
}

void init()
{
    for (int i = 1; i < MAXN; i++)
    {
        divisors[i] = get_divisors(i);
        for (const auto &u: divisors[i])
        {
            multiples[u].push_back(i);
        }
    }

    for (int i = 1; i < MAXN; i++)
    {
        tree[i] = multiple_segment_tree(multiples[i]);
    }
}


int main()
{   
    cin.sync_with_stdio(false);
    cin.tie(NULL);
    init();

    int n;
    cin >> n;
    for (int i = 0; i < n; i++)
    {
        string cmd;
        long long P, N;
        cin >> cmd >> P >> N;
        if (cmd == "ADD")
        {
            for (const auto &u: divisors[P])
            {
                tree[u].add(P, N);
            }
        }
        else if (cmd == "RM")
        {
            for (const auto &u: divisors[P])
            {
                tree[u].add(P, -1 * N);
            }
        }
        else
        {
            cout << tree[P].query(N) << "\n";
        }
    }
    return 0;
}
