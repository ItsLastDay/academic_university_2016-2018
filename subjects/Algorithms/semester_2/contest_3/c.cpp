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

const int ALPHABET_SIZE = 257;
const int MAXN = 200500;

struct suffix_array
{
    suffix_array(vector<int> const& a)
        : s(a)
    {
        build_sa();
        build_lcp();
    }

    int get_suffix_at(int idx) const
    {
        return sa[idx];
    }

    int get_lcp_at(int idx) const
    {
        if (idx == -1 || idx == s.size())
            return -1;
        return lcp[idx];
    }

    vector<int> get_concrete_cyclic_at(int idx) const
    {
        vector<int> res;
        for (int i = 0; i < s.size(); i++)
            res.push_back(s[(sa[idx] + i) % s.size()]);
        return res;
    }

private:
    void build_sa()
    {
        int n = s.size();
        sa.assign(n, 0);
        vector<int> cnt;
        vector<int> cls;
        cls.assign(n, 0);
        cnt.assign(ALPHABET_SIZE, 0);

        for (int i = 0; i < n; i++)
        {
            cnt[s[i]]++;
        }
        for (int i = 1; i < ALPHABET_SIZE; i++)
        {
            cnt[i] += cnt[i - 1];
        }
        for (int i = n - 1; i >= 0; i--)
        {
            sa[--cnt[s[i]]] = i;
        }

        int classes = 1;
        cls[sa[0]] = 0;

        for (int i = 1; i < n; i++)
        {
            if (s[sa[i]] != s[sa[i - 1]])
                classes++;
            cls[sa[i]] = classes - 1;
        }

        vector<int> prev_idx(n);
        vector<int> cls2(n);
        cnt.assign(n, 0);

        for (int it = 0; (1 << it) <= n; it++)
        {
            // Already sorted suffixes of length (1 << i).
            // Now want to sort (1 << (i + 1)).
            
            for (int i = 0; i < n; i++)
            {
                prev_idx[i] = sa[i] - (1 << it);
                while (prev_idx[i] < 0)
                    prev_idx[i] += n;
            } 

            fill(cnt.begin(), cnt.begin() + n, 0);
            for (int i = 0; i < n; i++)
            {
                cnt[cls[sa[i]]]++;
            }
            for (int i = 1; i < n; i++)
                cnt[i] += cnt[i - 1];

            // Sort by first element of pair!
            for (int i = n - 1; i >= 0; i--)
            {
                sa[--cnt[cls[prev_idx[i]]]] = prev_idx[i];
            }

            classes = 1;
            cls2[sa[0]] = 0;
            for (int i = 1; i < n; i++)
            {
                int nxt1 = (sa[i] + (1 << it)) % n;
                int nxt2 = (sa[i - 1] + (1 << it)) % n;
                if (cls[sa[i]] != cls[sa[i - 1]] or cls[nxt1] != cls[nxt2])
                   classes++;
                cls2[sa[i]] = classes - 1; 
            }

            if (classes == n)
                break;

            cls = cls2;
        }

        inv_sa.assign(n, 0);
        for (int i = 0; i < n; i++)
            inv_sa[sa[i]] = i;
    }

    void build_lcp()
    {
        int n = s.size();
        lcp.assign(n, 0);

        int cur_lcp = 0;
        for (int i = 0; i < n; i++)
        {
            int j = inv_sa[i];

            if (j != n - 1)
            {
                int pos = sa[j + 1];
                while (pos + cur_lcp < n and i + cur_lcp < n and
                        s[i + cur_lcp] == s[pos + cur_lcp])
                    cur_lcp++;
                lcp[j] = cur_lcp;
            }

            cur_lcp = max(cur_lcp - 1, 0);
        }
    }

    vector<int> lcp;
    vector<int> sa, inv_sa;
    vector<int> s;
};

int main()
{   
    cin.sync_with_stdio(false);
    cin.tie(NULL);

    int n, _;
    cin >> n >> _;
    vector<int> a(n + 1, 11);
    for (int i = 0; i < n; i++)
        cin >> a[i];

    auto const sa = suffix_array(a);

    /*
    for (int i = 0; i < n; i++)
    {
        cout << sa.get_lcp_at(i) << ": ";
        for (const auto &u: sa.get_concrete_cyclic_at(i))
            cout << u << " ";
        cout << endl;
    }
    cout << endl;
    */

    long long max_value = n;
    int best_len = n;
    int best_start = 0;

    stack<int> st;
    st.push(-1);

    vector<int> lowest(n);
    vector<int> highest(n);

    for (int i = 0; i < n; i++)
    {
        int now_lcp = sa.get_lcp_at(i);
        while (sa.get_lcp_at(st.top()) >= now_lcp)
        {
            st.pop();
        }
        lowest[i] = st.top();
        st.push(i);
    }

    while (not st.empty())
        st.pop();

    st.push(n + 1);
    for (int i = n - 1; i >= 0; i--)
    {
        int now_lcp = sa.get_lcp_at(i);
        while (sa.get_lcp_at(st.top()) >= now_lcp)
        {
            st.pop();
        }
        highest[i] = st.top();
        st.push(i);
    }

    for (int i = 0; i < n; i++)
    {
        int now_lcp = sa.get_lcp_at(i);
        int now_occ = highest[i] - lowest[i];
        long long now_val = static_cast<long long>(now_lcp) * now_occ;

//        cout << i << " " << lowest[i] <<  " " << highest[i] << endl;

        if (now_val > max_value)
        {
            max_value = now_val;
            best_len = now_lcp;
            best_start = sa.get_suffix_at(i);
        }
    }

    cout << max_value << "\n" << best_len << "\n";
    for (int i = best_start; i < best_start + best_len; i++)
        cout << a[i] << " ";
    return 0;
}