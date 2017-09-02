#include <bits/stdc++.h>
using namespace std;
using namespace std::chrono;
#define DBG(x) cout << #x << " = " << x << endl;
/*
Compile:
    -Wall -Wextra -pedantic -std=c++14 -O2 -Wshadow -Wformat=2 -Wfloat-equal -Wconversion -Wlogical-op -Wcast-qual -Wcast-align -fwhole-program -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -lmcheck -fsanitize=address 

    -std=c++11 -O2
 
Check for memory usage:
    valgrind --tool=massif
*/

const int MAXN = 100500;
const int INF = MAXN;

enum
{
    OPEN, 
    ADD,
    INC,
    CLOSE,
    DELETE,
};

long long gcd(long long a, long long b)
{
    while (b)
    {
        long long t = b;
        b = a % b;
        a = t;
    }
    return a;
}


struct sum_segtree
{
    vector<long long> tr;
    int n;

    sum_segtree(int n_)
    {
        n = n_;
        tr.assign(4 * (n + 1), 0);
    }

    long long get(int q_le, int q_rg)
    {
        return get(1, 0, n - 1, q_le, q_rg);
    }

    void add(int idx, long long new_val)
    {
        update(1, 0, n - 1, idx, new_val);
    }

private:
    long long get(int v, int le, int rg, int q_le, int q_rg)
    {
        if (q_rg < le or q_le > rg)
            return 0;
        if (q_le <= le and rg <= q_rg)
        {
            return tr[v];
        }

        return get(v * 2, le, (le + rg) / 2, q_le, q_rg) +
            get(v * 2 + 1, (le + rg) / 2 + 1, rg, q_le, q_rg);
    }

    void update(int v, int le, int rg, int idx, long long new_val)
    {
        if (not (le <= idx and idx <= rg))
            return;
        if (le == rg)
        {
            tr[v] += new_val;
            return;
        }

        update(v * 2, le, (le + rg) / 2, idx, new_val);
        update(v * 2 + 1, (le + rg) / 2 + 1, rg, idx, new_val);
        tr[v] = tr[v * 2] + tr[v * 2 + 1];
    }
};

struct max_segtree
{
    vector<pair<int, int>> tr;
    int n;

    max_segtree(int n_)
    {
        n = n_;
        tr.resize(4 * (n + 2));
        build(1, 0, n - 1);
    }

    void build(int v, int le, int rg)
    {
        tr[v] = make_pair(-1, le);
        if (le != rg)
        {
            build(2 * v, le, (le + rg) / 2);
            build(2 * v + 1, (le + rg) / 2 + 1, rg);
        }
    }

    void update(int idx, int new_val)
    {
        update(1, 0, n - 1, idx, new_val);
    }

    pair<int, int> get(int q_le, int q_rg)
    {
        auto res = get(1, 0, n - 1, q_le, q_rg);
        return res;
    }

private:
    pair<int, int> get(int v, int le, int rg, int q_le, int q_rg)
    {

        if (q_rg < le or q_le > rg)
            return make_pair(-1, -1);
        if (q_le <= le and rg <= q_rg)
        {
            return tr[v];
        }

        return max(get(v * 2, le, (le + rg) / 2, q_le, q_rg),
                get(v * 2 + 1, (le + rg) / 2 + 1, rg, q_le, q_rg));
    }

    void update(int v, int le, int rg, int idx, int new_val)
    {
        if (not (le <= idx and idx <= rg))
            return;
        if (le == rg)
        {
            tr[v] = make_pair(new_val, le);
            return;
        }

        update(v * 2, le, (le + rg) / 2, idx, new_val);
        update(v * 2 + 1, (le + rg) / 2 + 1, rg, idx, new_val);
        tr[v] = max(tr[v * 2], tr[v * 2 + 1]);
    }
};

struct min_segtree
{
    vector<int> tr;
    int n;

    min_segtree(int n_)
    {
        n = n_;
        tr.assign(4 * (n + 1), INF);
    }

    void update(int idx, int new_val)
    {
        update(1, 1, n, idx, new_val);
    }

    int get(int q_le, int q_rg)
    {
        return get(1, 1, n, q_le, q_rg);
    }

private:
    int get(int v, int le, int rg, int q_le, int q_rg)
    {

        if (q_rg < le or q_le > rg)
            return INF;
        if (q_le <= le and rg <= q_rg)
        {
            return tr[v];
        }

        return min(get(v * 2, le, (le + rg) / 2, q_le, q_rg),
                get(v * 2 + 1, (le + rg) / 2 + 1, rg, q_le, q_rg));
    }

    void update(int v, int le, int rg, int idx, int new_val)
    {
        if (not (le <= idx and idx <= rg))
            return;
        if (le == rg)
        {
            tr[v] = new_val;
            return;
        }

        update(v * 2, le, (le + rg) / 2, idx, new_val);
        update(v * 2 + 1, (le + rg) / 2 + 1, rg, idx, new_val);
        tr[v] = min(tr[v * 2], tr[v * 2 + 1]);
    }
};

struct strange_tree_mock
{
    vector<long long> current_number;
    vector<long long> sum;
    vector<int> count;
    int n;

    strange_tree_mock(int n_)
    {
        n = n_;
        current_number.assign(4 * (n + 2), 0);
        sum.assign(4 * (n + 2), 0);
        count.assign(4 * (n + 2), 0);
    }

    void increase_count(int q_le, int q_rg, int inc)
    {
        for (int i = q_le; i <= q_rg; i++)
        {
            count[i] += inc;
        }
    }

    long long get_sum(int idx)
    {
        return sum[idx] + count[idx] * current_number[idx];
    }

    void change_current_number(int q_le, int q_rg, int new_val)
    {
        for (int i = q_le; i <= q_rg; i++)
        {
            sum[i] += count[i] * current_number[i];
            count[i] = 0;
            current_number[i] = new_val;
        }
    }
};

struct strange_tree
{
    vector<long long> current_number;
    vector<long long> deferred;
    vector<long long> sum;
    vector<long long> add;
    vector<long long> count;
    int n;

    strange_tree(int n_)
    {
        n = n_;
        current_number.assign(4 * (n + 2), 0);
        deferred.assign(4 * (n + 2), -1);
        sum.assign(4 * (n + 2), 0);
        add.assign(4 * (n + 2), 0);
        count.assign(4 * (n + 2), 0);
    }


    void increase_count(int q_le, int q_rg, int inc)
    {
        increase_count(1, 0, n - 1, q_le, q_rg, inc);
    }

    void change_current_number(int q_le, int q_rg, int new_val)
    {
        change_current_number(1, 0, n - 1, q_le, q_rg, new_val);
    }
    
    long long get_sum(int idx)
    {
        return get_sum(1, 0, n - 1, idx);
    }

private: 
    long long get_sum(int v, int le, int rg, int idx)
    {
        push(v, le, rg);

        if (not (le <= idx and idx <= rg))
        {
            return 0;
        }
        if (le == idx)
        {
            return sum[v];
        }

        return get_sum(v * 2, le, (le + rg) / 2, idx) +
            get_sum(v * 2 + 1, (le + rg) / 2 + 1, rg, idx);
    }

    void change_current_number(int v, int le, int rg, int q_le, int q_rg, int new_val)
    {
        push(v, le, rg);

        if (q_le > rg or q_rg < le)
        {
            return;
        }
        if (q_le <= le and rg <= q_rg)
        {
            deferred[v] = new_val;
            return;
        }

        change_current_number(v * 2, le, (le + rg) / 2, q_le, q_rg, new_val);
        change_current_number(v * 2 + 1, (le + rg) / 2 + 1, rg, q_le, q_rg, new_val);
    }

    void increase_count(int v, int le, int rg, int q_le, int q_rg, int inc)
    {
        push(v, le, rg);

        if (q_le > rg or q_rg < le)
        {
            return;
        }
        if (q_le <= le and rg <= q_rg)
        {
            count[v] = inc;
            return;
        }

        increase_count(v * 2, le, (le + rg) / 2, q_le, q_rg, inc);
        increase_count(v * 2 + 1, (le + rg) / 2 + 1, rg, q_le, q_rg, inc);
    }

    void push(int v, int le, int rg)
    {
        (void)le;
        (void)rg;

        sum[v] += count[v] * current_number[v];
        if (deferred[v] != -1)
        {
            current_number[v] = deferred[v];
        }

        if (v * 2 < static_cast<int>(current_number.size()))
        {
            add[v * 2] += add[v];
            if (deferred[v] != -1)
            {
                deferred[v * 2] = deferred[v];
            }
        }

        if (v * 2 + 1 < static_cast<int>(current_number.size()))
        {
            add[v * 2 + 1] += add[v];
            if (deferred[v] != -1)
                deferred[v * 2 + 1] = deferred[v];
        }

        deferred[v] = -1;
        add[v] = 0;
    }
    
};


void solve(int n, int k)
{
    set<int> all_xs;
    unordered_map<int, int> x_to_idx;
    unordered_map<int, int> idx_to_x;
    all_xs.insert(0);
    unordered_map<int, set<int>> x_occurencies;

    vector< tuple<int, int, int, int> > events;
    events.reserve(k * 2);
    vector<long long> answers(k + 1);
    vector<bool> need_answer(k + 1, false);
    vector<long long> lenghts(k + 1);
    idx_to_x[-1] = 0;

    events.emplace_back(0, ADD, 0, 0);
    for (int i = 1; i <= k; i++)
    {
        string cmd;
        cin >> cmd;

        if (cmd == "^")
        {
            int fr, to, val;
            cin >> fr >> to >> val;

            all_xs.insert(val);
            events.emplace_back(fr, ADD, val, i);
            events.emplace_back(to, DELETE, val, i);
        }
        else
        {
            int fr, to;
            cin >> fr >> to;
            events.emplace_back(fr, OPEN, 0, i);
            events.emplace_back(to, CLOSE, 0, i);
            lenghts[i] = to - fr + 1;
            need_answer[i] = true;
        }
    }

    int idx_xs = 1;
    for (const auto& x: all_xs)
    {
        x_to_idx[x] = idx_xs;
        idx_to_x[idx_xs] = x;
        ++idx_xs;
    }

    for (int i = 1; i <= n; i++)
    {
        events.emplace_back(i, INC, 0, 0);
    }

    sort(events.begin(), events.end());

    min_segtree min_time(idx_xs);
    max_segtree max_x(k + 2);
    strange_tree sum_vals(k + 2);

    sum_vals.change_current_number(1, k, 1);
    sum_vals.increase_count(0, k, 1);
    DBG(sum_vals.get_sum(2));
    exit(0);

    for (const auto& ev: events)
    {
        int temp, type, val, tm;
        tie(temp, type, val, tm) = ev;

        if (type == INC)
        {
            DBG("INC");
            sum_vals.increase_count(0, k, 1);
        }
        else if (type == OPEN)
        {
            answers[tm] = -1 * sum_vals.get_sum(tm);
#ifdef DEBUG_OPEN
            DBG("open");
            DBG(tm);
            DBG(answers[tm]);
            for (int i = 0; i <= k; i++)
            {
                cout << time_sum.get(0, i) << " ";
            }
            cout << endl;
            cout << endl;
#endif
        }
        else if (type == CLOSE)
        {
            answers[tm] += sum_vals.get_sum(tm);
#ifdef DEBUG_CLOSE
            DBG("CLOSE");
            DBG(tm);
            DBG(answers[tm]);
            for (int i = 0; i <= k; i++)
            {
                cout << time_sum.get(0, i) << " ";
            }
            cout << endl;
            cout << endl;
#endif
        }
        else if (type == ADD)
        {
#ifdef DEBUG_ADD
            DBG("ADD");
#endif
            int idx = x_to_idx[val];
            int tH = min_time.get(idx, idx_xs - 1);
            tH = min(tH, k + 1);
#ifdef DEBUG_ADD
            DBG(idx);
            DBG(tH);
#endif

            if (tH > tm)
            {
                int was_val = max_x.get(0, tm).first;
                was_val = max(was_val, 0);
                sum_vals.change_current_number(tm, tH - 1, val);
            }

            x_occurencies[val].insert(tm);
            min_time.update(idx, *(x_occurencies[val].begin()));
            max_x.update(tm, val);

#ifdef DEBUG_ADD
            DBG(val);
            DBG(tm);
            DBG(temp);
            for (int i = 0; i <= k; i++)
            {
                cout << sum_vals.current_number[i] << " ";
            }
            cout << endl;
            for (int i = 1; i < idx_xs; i++)
            {
                cout << min_time.get(i, i) << " ";
            }
            cout << endl;
            for (int i = 0; i <= k; i++)
            {
                cout << max_x.get(i, i).first << " ";
            }
            cout << endl << endl;
#endif
        }
        else if (type == DELETE)
        {
            int idx = x_to_idx[val];
            x_occurencies[val].erase(tm);
            max_x.update(tm, -1);
            min_time.update(idx, x_occurencies[val].empty() ? INF : *(x_occurencies[val].begin()));

#ifdef DEBUG_DELETE
            DBG("DELETE");
#endif
            int tG = min_time.get(idx, idx_xs - 1);
            tG = min(tG, k + 1);

            if (tG > tm)
            {
                int cur_end = tG - 1;
                while (cur_end > tm)
                {
                    auto new_val = max_x.get(0, cur_end);
#ifdef DEBUG_DELETE
                    DBG(new_val.first);
                    DBG(new_val.second);
#endif
                    if (new_val.first == -1)
                    {
                        break;
                    }
                    int split_idx = new_val.second;
                    sum_vals.change_current_number(split_idx, cur_end, new_val.first);
                    cur_end = split_idx - 1;
                }
            }

            max_x.update(tm, -1);

#ifdef DEBUG_DELETE
            DBG(val);
            DBG(tG);
            DBG(tm);
            for (int i = 0; i <= k; i++)
            {
                cout << sum_vals.current_number[i] << " ";
            }
            cout << endl;
#endif
        }
        else assert(false);

#ifdef DEBUG_TREE
        DBG(type);
        DBG(temp);
        DBG(val);
        DBG(tm);
        for (int i = 0; i <= k; i++)
        {
            cout << sum_vals.get_sum(i) << " ";
        }
        cout << endl << endl;
#endif
    }

    for (int i = 1; i <= k; i++)
    {
        if (not need_answer[i])
            continue;

        long long sum = answers[i];
        long long sz = lenghts[i];

        long long g = gcd(sum, sz);
        sum /= g;
        sz /= g;
        if (sz == 1)
            cout << sum << '\n';
        else 
            cout << sum << '/' << sz << '\n';
    }
}

int main()
{   
    cin.sync_with_stdio(false);
    cin.tie(NULL);

    int n, k;
    while (cin >> n >> k)
    {
        if (n == 0 and k == 0)
            break;
        solve(n, k);
    }
    return 0;
}
