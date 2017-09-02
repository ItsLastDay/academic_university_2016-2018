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

int rev[1005000];

int p;
using matrix_t = vector<vector<long long>>;

void print_mat(vector<vector<long long>> const& x)
{
    int nn = x.size();
    int mm = x[0].size();
    for (int i = 0; i < nn; i++)
        for (int j = 0; j < mm; j++)
            cout << setw(1) << x[i][j] << " \n"[j == mm - 1];
}

matrix_t transpose(matrix_t const& x)
{
    int n = x.size();
    int m = x[0].size();
    matrix_t result(m);
    for (int i = 0; i < m; i++)
        result[i].resize(n);

    for (int i = 0; i < n; i++)
       for (int j = 0; j < m; j++)
          result[j][i] = x[i][j];

    return result;
}


matrix_t mult(matrix_t const& a, matrix_t const& b)
{
    int n = a.size();
    int m = b[0].size();
    int k = a[0].size();
    matrix_t result(n);
    for (int i = 0; i < n; i++)
        result[i].resize(m);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
        {
            result[i][j] = 0;
            for (int r = 0; r < k; r++)
            {
                result[i][j] += a[i][r] * b[r][j] % p;
                result[i][j] %= p;
            }
        }
    return result;
}

matrix_t gauss_inverse(matrix_t const &orig, matrix_t & main_mat)
{
    int n = orig.size();
    int m = orig[0].size();
    matrix_t a(n);
    for (int i = 0; i < n; i++)
        a[i].resize(2 * m);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < 2 * m; j++)
            a[i][j] = 0;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
            a[i][j] = orig[i][j];
        a[i][min(i, m - 1) + m] = 1;
    }

    int row_idx = 0;
    for (int i = 0; i < n; i++)
    {
        int mx_idx = row_idx;
        for (int j = row_idx; j < n; j++)
        {
            if (a[j][i] > a[mx_idx][i] and a[mx_idx][i] == 0)
                mx_idx = j;
        }
        if (a[mx_idx][i] == 0)
            continue;

        for (int j = 0; j < 2 * m; j++)
        {
            swap(a[mx_idx][j], a[row_idx][j]);
        }

        for (int j = row_idx + 1; j < n; j++)
        {
            if (a[j][i] != 0)
            {
                long long sp = rev[a[row_idx][i]] * static_cast<long long>(a[j][i]) % p;
                for (int k = 0; k < 2 * m; k++)
                {
                    a[j][k] = (a[j][k] - sp * a[row_idx][k] % p + p) % p;
                }
            }
        }
        ++row_idx;
    }

    for (int i = n - 1; i >= 0; i--)
    {
        long long pv = a[i][i];
        for (int j = 0; j < 2 * m; j++)
        {
            a[i][j] = (a[i][j] * rev[pv]) % p;
        }

        for (int j = i + 1; j < n; j++)
        {
            if (a[i][j] == 0)
                continue;
            long long mu = a[i][j];
            for (int k = j; k < 2 * m; k++)
            {
                a[i][k] = (a[i][k] - a[j][k] * mu % p + p) % p;
            }
        }
    }

    matrix_t result(n);
    for (int i = 0; i < n; i++)
    {
        result[i].resize(m);
        for (int j = 0; j < n; j++)
            result[i][j] = a[i][j + m];
    }

    main_mat.resize(n);
    for (int i = 0; i < n; i++)
    {
        main_mat[i].resize(m);
        for (int j = 0; j < m; j++)
            main_mat[i][j] = a[i][j];
    }

    return result;
}

const int MAXN = 303 * 2;

int n, m;
long long mat[MAXN][MAXN];
long long tmp[MAXN][MAXN];
long long orig_mat[MAXN][MAXN];

void transpose()
{
    memcpy(tmp, mat, sizeof(tmp));
    for (int i =1; i <= n; i++)
        for (int j = 1; j <= m; j++)
            mat[j][i] = tmp[i][j];
    swap(n, m);
}

void swap_rows(int x, int y)
{
    for (int i = 1; i <= m; i++)
        swap(mat[x][i], mat[y][i]);
}

void print_mat()
{
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= m; j++)
        {
            cout << setw(1) << mat[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void solve()
{
    matrix_t original_matrix(n);
    for (int i = 0; i < n; i++)
    {
        original_matrix[i].resize(m);
        for (int j = 0; j < m; j++)
            original_matrix[i][j] = mat[i + 1][j + 1];
    }

    int row_idx = 1;
    for (int i = 1; i <= m and row_idx <= n; i++)
    {
        int mx_idx = row_idx;
        for (int j = row_idx; j <= n; j++)
        {
            if (mat[j][i] > mat[mx_idx][i] and mat[mx_idx][i] == 0)
                mx_idx = j;
        }
        if (mat[mx_idx][i] == 0)
            continue;

        swap_rows(mx_idx, row_idx);
        for (int j = row_idx + 1; j <= n; j++)
        {
            if (mat[j][i] != 0)
            {
                long long sp = rev[mat[row_idx][i]] * static_cast<long long>(mat[j][i]) % p;
                for (int k = 1; k <= m; k++)
                {
                    mat[j][k] = (mat[j][k] - sp * mat[row_idx][k] % p + p) % p;
                }
            }
        }
        ++row_idx;
    }

    unordered_map<int, int> to_pivot;
    for (int i = n; i >= 1; i--)
    {
        int leftmost = 0;
        for (int j = 1; j <=  m; j++)
        {
            if (mat[i][j] != 0)
            {
                leftmost = j;
                break;
            }
        }
        if (leftmost == 0)
            continue;
        to_pivot[leftmost] = i;

        long long pv = mat[i][leftmost];
        for (int j = 1; j <=  m; j++)
        {
            mat[i][j] = (mat[i][j] * rev[pv]) % p;
        }

#ifdef DEBUG
        DBG(i);
        print_mat();
#endif

        for (int j = leftmost + 1; j <= m; j++)
        {
            if (mat[i][j] == 0)
                continue;
            int other_row = to_pivot[j];
            if (other_row == 0)
                continue;
#ifdef DEBUG
            DBG(j);
            DBG(other_row);
#endif

            long long mu = mat[i][j];
            for (int k = j; k <= m; k++)
            {
                mat[i][k] = (mat[i][k] - mat[other_row][k] * mu % p + p) % p;
            }
        }

#ifdef DEBUG
        DBG("after");
        print_mat();
#endif
    }

    int rank = n;

    for (int i = n; i >= 1; i--)
    {
        bool any_nzero = false;
        for (int j = 1; j <= m; j++)
            any_nzero |= mat[i][j] != 0;
        if (any_nzero)
            break;
        rank--;
    }

    matrix_t second_mat(rank);
    for (int i = 0; i < rank; i++)
    {
        second_mat[i].resize(m);
        for (int j = 0; j < m; j++)
            second_mat[i][j] = mat[i + 1][j + 1];
    }

    cout << rank << "\n";
    vector<bool> pivots(m + 1);
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= m; j++)
        {
            if (mat[i][j] != 0)
            {
                pivots[j] = true;
                break;
            }
        }
    }

    vector<vector<long long>> first_mat;
    for (int i = 1; i <= n; i++)
    {
        first_mat.push_back(vector<long long>());
        for (int j = 1; j <= m; j++)
        {
            if (pivots[j])
                first_mat.back().push_back(orig_mat[i][j]);
        }
    }

    print_mat(first_mat);
    print_mat(second_mat);
#ifdef DEBUG
    cout << endl;

    print_mat(mult(first_mat, second_mat));
    cout << endl;
    print_mat(original_matrix);
    cout << endl;
    assert(mult(first_mat, second_mat) == original_matrix);
#endif
}

int main()
{   
    cin.sync_with_stdio(false);
    cin.tie(NULL);

    cin >> n >> m >> p;

    assert(p != 1);

    rev[1] = 1;

    for (int i = 2; i < p; i++)
    {
        rev[i] = (p - (p / i) * static_cast<long long>(rev[p % i]) % p) % p;
    }

    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++)
            cin >> mat[i][j];

    memcpy(orig_mat, mat, sizeof(mat));
    solve();
    return 0;
}
