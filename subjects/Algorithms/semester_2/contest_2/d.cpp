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
hash_t p_pow[100500];

void init()
{
    p_pow[0] = hashing(1);
    for (int i = 1; i < 100500; i++)
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

    string s, t;
    cin >> s >> t;

    auto hs = build_hash(s);
    auto ht = build_hash(t);

    int n = s.size();
    int m = t.size();

    vector<int> answer;
    for (int i = 0; i + m - 1 < n; i++)
    {
        if (get(i, i + m - 1, hs) == get(0, m - 1, ht))
            answer.push_back(i + 1);
    }

    cout << answer.size() << "\n";
    for (const auto &u: answer)
        cout << u << " " ;
    return 0;
}
