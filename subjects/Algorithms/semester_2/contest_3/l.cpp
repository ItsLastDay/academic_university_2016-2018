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

const int MAXN = 2010;

using mat_row_t = bitset<MAXN>;

mat_row_t mat[MAXN];

void no()
{
    cout << "NO" << endl;
    exit(0);
}

int main()
{   
    cin.sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;
    for (int i = 0; i < n; i++)
    {
        string s;
        cin >> s;
        for (int j = 0; j < n; j++)
            mat[i].set(j, s[j] == '1');
    }

    for (int i = n; i < 2 * n; i++)
    {
        for (int j = 0; j < n; j++)
            mat[j].set(i, j == i - n);
    }


    for (int j = 0; j < n; j++)
    {
        int max_idx = j;
        for (int i = j; i < n; i++)
        {
            if (mat[i][j] > mat[max_idx][j])
                max_idx = i;
        }

        if (not mat[max_idx][j])
            no();

        swap(mat[max_idx], mat[j]);

        for (int i = j + 1; i < n; i++)
        {
            if (mat[i][j])
                mat[i] ^= mat[j];
        }
    }

    for (int j = n - 1; j >= 0; j--)
    {
        for (int i = j - 1; i >= 0; i--)
        {
            if (mat[i][j])
                mat[i] ^= mat[j];
        }
    }

    for (int i = 0; false and i < n; i++)
    {
        for (int j = 0; j < 2 * n; j++)
            cout << (mat[i][j] ? '1' : '0');
        cout << endl;
    }

    cout << "YES\n";
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cout << (mat[i][j + n] ? '1' : '0');
        }
        cout << "\n";
    }
    return 0;
}
