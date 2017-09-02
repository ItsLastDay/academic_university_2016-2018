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

using comp = complex<double>;

vector<comp> fft(vector<comp> a, comp w)
{
    // Highest coef. first.
    int n = (int)a.size();
    if (n == 1)
        return a;
    if (n == 4)
    {
        vector<comp> res(n);
        comp base_w = w;
        for (int i = 0; i < n; i++)
        {
            res[i] = a[0] * pow(w, 3) + a[1] * pow(w, 2) + a[2] * w + a[3];
            w *= base_w;
        }
        return res;
    }

    vector<comp> odd(n / 2), even(n / 2);
    for (int i = 0; i < n; i++)
    {
        if (not (i & 1))
            odd[i / 2] = a[i];
        else
            even[i / 2] = a[i];
    }

    auto res_odd = fft(move(odd), w * w);
    auto res_even = fft(move(even), w * w);

    comp w_cur = w;
    for (int i = 0; i < n / 2; i++)
    {
        a[i] = res_even[i] + w_cur * res_odd[i];
        a[i + n / 2] = res_even[i] - w_cur * res_odd[i]; 
        w_cur *= w;
    }

    return a;
}

int main()
{   
    cin.sync_with_stdio(false);
    cin.tie(NULL);

    string s;
    cin >> s;
    string s_original = s;
    while (true)
    {
        int n = (int)s.size();
        if (not (n & (n - 1)))
            break;
        s += "0";
    }

    long long answer = 0;

    for (int it = 0; it < 2; it++) 
    {
        int n = (int)s.size();
        vector<comp> a(2 * n, 0), b(2 * n, 0);
        int pl = n - 1;
        for (int i = n - 1; i >= 0; i--)
        {
            a[pl] = s[i] == '1';
            pl--; 
        } 

        for (int i = 0; i < n; i++)
        {
            b[i] = s[i] == '1';
        }

        comp w = polar<double>(1, M_PI / n);
        auto res_a = fft(a, w);
        auto res_b = fft(b, w);

        vector<comp> result(2 * n, 0);
        for (int i = 0; i < 2 * n; i++)
            result[i] = res_a[i] * res_b[i];

        auto inv = fft(result, pow(w, -1));
        for (int i = 0; i < 2 * n; i++)
        {
            inv[i] /= 2 * n;
        }

#ifdef DEBUG
        for (int i = 0; i < 2 * n; i++)
        {
            auto val_i = static_cast<int>(round(inv[i].real()));
            cout << i << " "<< val_i << endl;
        }
#endif

        int begin = it == 0 ? s_original.size() : 2 * n - s_original.size();
        int end = begin + s_original.size();
#ifdef DEBUG
        DBG(begin);
        DBG(end);
#endif
        for (int i = begin; i < end; i++)
        {
            if ((end - 1 - i) % 2 == 0 and 
                    s_original[s_original.size() - 1 - (end - 1 - i) / 2] == '1')
            {
                answer += static_cast<int>(round(inv[i].real())) - 1;
            }
        }
#ifdef DEBUG
        DBG(answer);
#endif
        reverse(s.begin(), s.end());
        reverse(s_original.begin(), s_original.end());
    }

    
#ifdef DEBUG
//    reverse(s_original.begin(), s_original.end());
    s = s_original;
    cout << endl;
    cout << s << endl;
    DBG(s.size());
    string srev = s;
    reverse(srev.begin(), srev.end());
    for (int i = 0; i < s.size(); i++)
    {
        int cur_val = 0;
        for (int j = 0; j < s.size() and i + j < s.size(); j++)
        {
            cur_val += s[s.size() - j - 1] == s[i + j] and s[i + j] == '1';
        }
        cout << string(i, ' ') + srev.substr(0, s.size() - i) << endl;
        cout << i << " " << cur_val << endl;
    }
#endif
    

#ifdef DEBUG
    long long real_ans = 0;
    for (int i = 0; i < s_original.size(); i++)
    {
        if (s_original[i] == '0')
            continue;
        for (int j = 1; j < s_original.size(); j++)
        {
            if (i - j < 0 or i + j >= s_original.size())
                break;
            real_ans += s_original[i - j] == s_original[i + j] and
                s_original[i + j] == '1';
        }
    }
    DBG(real_ans);
    DBG(answer);
#endif

    if (s_original.size() & 1)
    {
        int i = s_original.size() / 2;
        if (s_original[i] == '1')
        for (int j = 1; j < s_original.size(); j++)
        {
            if (i - j < 0 or i + j >= s_original.size())
                break;
            answer -= 2 * (s_original[i - j] == s_original[i + j] and
                s_original[i + j] == '1');
        }
    }

    answer /= 2;
    cout << answer << endl;
#ifdef DEBUG
    DBG(answer);
    assert(real_ans == answer);
#endif
    return 0;
}

