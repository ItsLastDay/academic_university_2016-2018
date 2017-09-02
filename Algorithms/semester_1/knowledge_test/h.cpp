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

const int MAXN = 15010;
pair<int, int> arr[3][MAXN];
int sz[3];
int precomp_mn_2[MAXN];

void get_array(int idx)
{
    int n;
    cin >> n;
    sz[idx] = n;
    for(int i = 0; i < n; i++) {
        int cur;
        cin >> cur;
        arr[idx][i] = make_pair(cur, i);
    }
    sort(arr[idx], arr[idx] + n);
}

int main()
{
    cin.sync_with_stdio(false);

    int s;
    cin >> s;

    for(int i = 0; i < 3; i++)
        get_array(i);

    for(int i = 0; i < sz[2]; i++) {
        precomp_mn_2[i] = arr[2][i].second;
        if(i > 0 and arr[2][i - 1].first == arr[2][i].first)
            precomp_mn_2[i] = min(precomp_mn_2[i], precomp_mn_2[i - 1]);
    }

    tuple<int, int, int> answer(MAXN, MAXN, MAXN);

    for(int i = 0; i < sz[0]; i++) {
        int k = sz[2] - 1;
        for(int j = 0; j < sz[1]; j++) {
            long long base = arr[0][i].first + arr[1][j].first;
            while(k >= 0 and base + arr[2][k].first > s)
                k--;
            if(k < 0)
                break;
            
            if(base + arr[2][k].first == s) {
                answer = min(answer, make_tuple(arr[0][i].second, arr[1][j].second,
                            precomp_mn_2[k]));

            }
        }
    }

    if(get<0>(answer) == MAXN)
        cout << -1;
    else
        cout << get<0>(answer) << " " << get<1>(answer) << " " << get<2>(answer);
    return 0;
}
