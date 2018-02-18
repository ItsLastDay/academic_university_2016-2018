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

struct hashing
{
    hashing(long long v1_, long long v2_)
        : v1(v1_)
        , v2(v2_)
    {
    }

    hashing(long long x)
        : v1(x)
        , v2(x)
    {
    }

    hashing() = default;
    hashing(hashing const&) = default;
    hashing(hashing &&) = default;

    hashing& operator=(hashing const&) = default;
    hashing& operator=(hashing &&) = default;

    hashing operator+(hashing const& other)
    {
        return hashing((v1 + other.v1) % MOD1,
                        (v2 + other.v2) % MOD2);
    }

    hashing& operator+=(hashing const& other)
    {
        *this = *this + other;
        return *this;
    }

    hashing operator-(hashing const& other)
    {
        return hashing((v1 - other.v1 + MOD1) % MOD1,
                        (v2 - other.v2 + MOD2) % MOD2);
    }

    hashing& operator-=(hashing const& other)
    {
        *this = *this - other;
        return *this;
    }

    hashing operator*(hashing const& other)
    {
        return hashing((v1 * other.v1) % MOD1,
                        (v2 * other.v2) % MOD2);
    }

    hashing& operator*=(hashing const& other)
    {
        *this = *this * other;
        return *this;
    }

    bool operator==(hashing const& other)
    {
        return v1 == other.v1 and v2 == other.v2;
    }
private:
    static const int MOD1 = 1e9 + 7;
    static const int MOD2 = 1e9 + 9;

    long long v1, v2;
};


const int MAXN = 100500;

hashing mult(100500, 100500 + 1);
hashing powers[MAXN];

int a[MAXN];
hashing ha[MAXN], harev[MAXN];

hashing get_hash(int start, int end)
{
    return ha[end] - ha[start - 1] * powers[end - start + 1];
}

hashing get_rev_hash(int start, int end)
{
    return harev[start];
}

int main()
{   
    cin.sync_with_stdio(false);
    cin.tie(NULL);

    powers[0] = 1;
    for (int i = 1; i < MAXN; i++)
        powers[i] = powers[i - 1] * mult;

    int n, m;
    cin >> n >> m;
    for (int i = 1; i <= n; i++)
    {
        cin >> a[n - i + 1];
    }

    for (int i = 1; i <= n; i++)
    {
        ha[i] = ha[i - 1] * mult + a[i];
    }

    for (int i = n; i >= 1; i--)
    {
        harev[i] = harev[i + 1] * mult + a[i];
    }

    for (int i = 1; i <= n; i++)
    {
        int len_to_cover = n - i;
        if (len_to_cover > i)
            continue;

        if (get_hash(i - len_to_cover + 1, i) ==
                get_rev_hash(i + 1, n))
            cout << i << " ";
    }
    return 0;
}
