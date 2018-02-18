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
const int INF = 1e9 + 123;

using ll = long long;


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


struct sqrt_block
{
    int n;
    vector< pair<int, int> > number_indices;
    vector<ll> pref_sum;
    int used_size;
    int starting_index_in_array;
    ll used_value;

    sqrt_block() = default;

    sqrt_block(int sz, int starting_index_)
    {
        n = sz;
        starting_index_in_array = starting_index_;
        clear();
    }

    void clear()
    {
        number_indices.clear();
        number_indices.resize(n + 1);
        pref_sum.assign(n + 1, 0);
        used_value = 0;
        used_size = 0;
    }

    void upd_max(int val)
    {
        if (val <= used_value)
        {
            return;
        }

        if (used_size == n)
        {
            used_value = val;
            return;
        }

        auto it = upper_bound(number_indices.begin() + used_size, 
                number_indices.end(), 
                make_pair(val, 0));
        int dd = it - number_indices.begin();
        --dd;

        used_size = dd;
        used_value = val;
    }

    ll get_sum(int end)
    {
        return pref_sum[end] - pref_sum[used_size] + used_value * used_size;
    }

    ll get_total_sum()
    {
        return get_sum(n);
    }

    void build(vector<int> const& numbers)
    {
        clear();

        for (int i = 0; i < n; i++)
        {
            int idx = starting_index_in_array + i;
            number_indices[i + 1] = make_pair(numbers[idx], idx);
        }
        number_indices[0] = make_pair(0, 0);

        sort(number_indices.begin(), number_indices.end());
        for (int i = 1; i <= n; i++)
        {
            pref_sum[i] = pref_sum[i - 1];
            if (number_indices[i].first != INF)
            {
                pref_sum[i] += number_indices[i].first;
            }
        } 
    }

    void dump(vector<int>& numbers)
    {
        for (int i = 1; i <= used_size; i++)
        {
            numbers[number_indices[i].second] = used_value;
        }

        for (int i = used_size + 1; i <= n; i++)
        {
            int idx_arr = number_indices[i].second;
            int val = number_indices[i].first;
            numbers[idx_arr] = val;
        }
    }
};


void solve(int n, int k)
{
    vector<int> numbers(n + 1, 0);
    
    const int BLOCK_SIZE = 200;

    while (n % BLOCK_SIZE != 0)
    {
        numbers.push_back(INF);
        n++;
    }

    vector<int> block_num(n + 1);
    vector<sqrt_block> blocks(n / BLOCK_SIZE);

    int bl_num = 0;
    for (int i = 1; i <= n; i += BLOCK_SIZE)
    {
        blocks[bl_num] = sqrt_block(BLOCK_SIZE, i);
        blocks[bl_num].build(numbers);

        for (int j = 0; j < BLOCK_SIZE; j++)
        {
            block_num[i + j] = bl_num;
        }
        ++bl_num;
    }


    for (int i = 0; i < k; i++)
    {
        string cmd;
        cin >> cmd;

        if (cmd == "^")
        {
            int fr, to, val;
            cin >> fr >> to >> val;
            
            int bl_left = block_num[fr];
            int bl_right = block_num[to];

            if (bl_left == bl_right)
            {
                blocks[bl_left].dump(numbers);

                for (int j = fr; j <= to; j++)
                {
                    numbers[j] = max(numbers[j], val);
                }

                blocks[bl_left].build(numbers);
            }
            else
            {
                blocks[bl_left].dump(numbers);
                for (int j = fr; block_num[j] == bl_left; j++)
                {
                    numbers[j] = max(numbers[j], val);
                }
                blocks[bl_left].build(numbers);

                blocks[bl_right].dump(numbers);
                for (int j = to; block_num[j] == bl_right; j--)
                {
                    numbers[j] = max(numbers[j], val);
                }
                blocks[bl_right].build(numbers);

                for (int j = bl_left + 1; j < bl_right; j++)
                {
                    blocks[j].upd_max(val);
                }
            }
        }
        else
        {
            int fr, to;
            cin >> fr >> to;
            long long sum = 0;

            int bl_left = block_num[fr];
            int bl_right = block_num[to];

            if (bl_left == bl_right)
            {
                blocks[bl_left].dump(numbers);
                for (int j = fr; j <= to; j++)
                {
                    sum += numbers[j];
                }
            }
            else
            {
                blocks[bl_left].dump(numbers);
                for (int j = fr; block_num[j] == bl_left; j++)
                {
                    sum += numbers[j];
                }

                blocks[bl_right].dump(numbers);
                for (int j = to; block_num[j] == bl_right; j--)
                {
                    sum += numbers[j];
                }

                for (int j = bl_left + 1; j < bl_right; j++)
                {
                    sum += blocks[j].get_total_sum();
                }
            }


            long long sz = to - fr + 1;
            long long g = gcd(sum, sz);
            sum /= g;
            sz /= g;
            if (sz == 1)
                cout << sum << '\n';
            else 
                cout << sum << '/' << sz << '\n';
        }
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
