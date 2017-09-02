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

const int MAXN = 200500;

int a[MAXN];
long long p[MAXN];

unordered_map<int, int> cnt;

int highest_idx[MAXN];

int main()
{   
    cin.sync_with_stdio(false);
    cin.tie(NULL);

    int n, k;
    cin >> n >> k;
    for (int i = 1; i <= n; i++)
    {
        cin >> a[i];
        p[i] = p[i - 1] + a[i];
    }

    for (int i = 0; i <= n; i++)
        highest_idx[i] = -1;

    int j = n;
    int current_unique = 0;
    for (int i = n; i > 0; i--)
    {
        while (j > 0 and current_unique < k)
        {
            cnt[a[j]]++;
            if (cnt[a[j]] == 1)
                current_unique++;

            j--;
        }

        if (current_unique < k)
            break;
        highest_idx[i] = j;
        cnt[a[i]]--;
        if (cnt[a[i]] == 0)
            current_unique--;
    }


    int idx_min = 0;
    bool seen_good = false;

    long long best_val = 0;
    int best_start, best_end;

    int to_j = 0;
    for (int i = 1; i <= n; i++)
    {
        if (highest_idx[i] == -1)
            continue;
        while (to_j <= highest_idx[i])
        {
            if (p[idx_min] > p[to_j])
                idx_min = to_j;
            to_j++;
        }

//        cout << i << "  " << idx_min << " " << highest_idx[i] << endl;
        long long now_val = p[i] - p[idx_min];
        if (seen_good == false or now_val > best_val)
        {
            seen_good = true;
            best_val = now_val;
            best_start = idx_min + 1;
            best_end = i;
        }
    }

    if (not seen_good)
        cout << "IMPOSSIBLE" << endl;
    else
    {
        cout << best_val << endl;
        cout << best_start << " " << best_end << endl;
    }
    return 0;
}
