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

const int MAXN = 222;

int n;
int start[MAXN];
vector<int> connected[MAXN];

int val[MAXN];
int mat[MAXN][MAXN];

void swap_rows(int x, int y)
{
    for (int i = 1; i <= n + 1; i++)
        swap(mat[x][i], mat[y][i]);
}

void print_mat()
{
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= n + 1; j++)
        {
            cout << mat[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

bool solve()
{
    int row_idx = 1;
    for (int i = 1; i <= n; i++)
    {
        int mx_idx = row_idx;
        for (int j = row_idx; j <= n; j++)
        {
            if (mat[j][i] > mat[mx_idx][i])
                mx_idx = j;
        }
        if (mat[mx_idx][i] == 0)
            continue;

        swap_rows(mx_idx, row_idx);
        for (int j = row_idx + 1; j <= n; j++)
        {
            if (mat[j][i] == 1)
            {
                for (int k = 1; k <= n + 1; k++)
                {
                    mat[j][k] ^= mat[row_idx][k];
                }
            }
        }
        ++row_idx;
    }

//    print_mat();

    int rank;
    for (rank = n; rank >= 1; rank--)
    {
        bool any_nzero = false;
        for (int j = 1; j <= n; j++)
            any_nzero |= mat[rank][j];
        if (mat[rank][n + 1] == 1 and not any_nzero)
            return false;
        if (any_nzero)
            break;
    }

    for (int i = n; i >= 1; i--)
    {
        int leftmost = 0;
        for (int j = 1; j <= n; j++)
        {
            if (mat[i][j] == 1)
            {
                leftmost = j;
                break;
            }
        }
        if (leftmost == 0)
            continue;

        for (int j = leftmost + 1; j <= n; j++)
        {
            if (mat[i][j])
                mat[i][n + 1] ^= val[j];
        }

        if (mat[i][leftmost])
        {
            val[leftmost] = mat[i][n + 1];
        }
        else
        {
            if (mat[i][n + 1] == 1)
                return false;
        }
    }

    return true;
}

void build_equations(int target)
{
    memset(mat, 0, sizeof(mat));

    for (int i = 1; i <= n; i++)
    {
        for (const auto&u : connected[i])
        {
            mat[u][i] ^= 1;
        }
        //mat[i][i] = 1;
    }
    
    for (int i = 1; i <= n; i++)
        mat[i][n + 1] = start[i] ^ target;
}

int main()
{   
    cin.sync_with_stdio(false);
    cin.tie(NULL);
    cin >> n;

    for (int i = 1; i <= n; i++)
    {
        int sz;
        cin >> sz;
        for (int j = 0; j < sz; j++)
        {
            int to;
            cin >> to;
            connected[i].push_back(to);
        }
    }

    for (int i = 1; i <= n; i++)
        cin >> start[i];

    for (int target = 0; target <= 1; target++)
    {
        build_equations(target);

        bool has_sol = solve();
        if (has_sol)
        {
            vector<int> answer;
            for (int i = 1; i <= n; i++)
            {
                if (val[i] == 1)
                    answer.push_back(i);
            }

            cout << answer.size() << "\n";
            for (const auto &u : answer)
                cout << u << " ";
            exit(0);
        }
    }

    cout << "-1" << endl;
    return 0;
}
