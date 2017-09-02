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

const int MOD1 = 1e9 + 7;
const int MOD2 = 1e9 + 9;

struct hashing
{
    hashing(int x)
        : v1(x)
        , v2(x)
    {
    }

    hashing(unsigned long long x1, unsigned long long x2)
        : v1(x1)
        , v2(x2)
    {
    }

    hashing() = default;

    bool operator==(hashing const& other) const
    {
        return v1 == other.v1 and v2 == other.v2;
    }

    hashing operator*(hashing const& other) const
    {
        return hashing(other.v1 * v1 % MOD1,
                other.v2 * v2 % MOD2);
    }

    hashing operator+(hashing const& other) const
    {
        return hashing((other.v1 + v1) % MOD1,
                (other.v2 + v2) % MOD2);
    }

    hashing operator-(hashing const& other) const
    {
        return hashing(
                (v1 - other.v1 + MOD1) % MOD1,
                (v2 - other.v2 + MOD2) % MOD2);
    }

private:
    unsigned long long v1, v2;
};

using hash_t = hashing;

hash_t P(37, 39);
hash_t p_pow[1005000];

void init()
{
    p_pow[0] = hashing(1);
    for (int i = 1; i < 1005000; i++)
        p_pow[i] = p_pow[i - 1] * P;
}

hash_t get(int le, int rg, vector<hash_t> const& val)
{
    if (le == 0)
        return val[rg];
    return val[rg] - val[le - 1] * p_pow[rg - le + 1];
}


vector<hash_t> build_hash(string const& s)
{
    vector<hash_t> result(s.size(), 0);

    for (size_t i = 0; i < s.size(); i++)
    {
        result[i] = s[i] - 'a' + 1;
        if (i > 0)
            result[i] = result[i] + result[i - 1] * P;
    }

    return result;
}

int main()
{   
    init();
    cin.sync_with_stdio(false);
    cin.tie(NULL);

    string s;
    cin >> s;
    auto hs = build_hash(s + s);

    auto cmp = 
            [&hs, &s](int x, int y)
            {
                if (s[x] != s[y])
                    return s[x] < s[y];
                int le = 1, rg = s.size() + 1;
                while (rg - le > 1)
                {
                    int mid = (le + rg) / 2;
                    if (get(x, x + mid - 1, hs) == get(y, y + mid - 1, hs))
                        le = mid;
                    else
                        rg = mid;
                }
                if (static_cast<size_t>(le) == s.size())
                    return false;
                return s[(x + le) % s.size()] < s[(y + le) % s.size()];
            };


    int answer = 0;

    for (size_t i = 1; i < s.size(); i++)
    {
        if (cmp(i, 0))
            answer++;
    }

    cout << answer + 1 << endl;
    return 0;
}
