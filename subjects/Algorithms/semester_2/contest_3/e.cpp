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

    int get_suffix_at(int idx)
    {
        return sa[idx];
    }

    int get_lcp_at(int idx)
    {
        return lcp[idx];
    }

    vector<int> get_concrete_cyclic_at(int idx)
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

        /*
        for (int i = 0; i < n; i++)
        {
            cout << get_suffix_at(i) << " " << cls[sa[i]] << ":  ";
            for (const auto &u: get_concrete_cyclic_at(i))
                cout << u << " ";
            cout << endl;
        }
        cout << endl;
        */

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

            /*
            DBG(it);
            for (int i = 0; i < n; i++)
            {
                cout << get_suffix_at(i) << " " << cls[sa[i]] << ":  ";
                for (const auto &u: get_concrete_cyclic_at(i))
                    cout << u << " ";
                cout << endl;
            }
            cout << endl;
            */
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

    int n;
    cin >> n;
    string s, t;
    cin >> s >> t;

    vector<int> a(2 * n + 2, 0);
    for (int i = 0; i < n; i++)
    {
        a[i] = s[i];
    }

    for (int i = 0; i < n; i++)
        a[n + 1 + i] = t[i];

    auto sa = suffix_array(a);

    int max_substr_len = 0;
    int max_beg = 0;
    for (int i = 2; i < 2 * n + 1; i++)
    {
        int cur = sa.get_suffix_at(i);
        int next = sa.get_suffix_at(i + 1);
        if ((cur < n) ^ (next < n))
        {
            int len = sa.get_lcp_at(i);
            if (len > max_substr_len)
            {
                max_substr_len = len;
                max_beg = cur < n ? cur : next;
            }
        }
    }

    cout << s.substr(max_beg, max_substr_len) << endl;
    return 0;
}
