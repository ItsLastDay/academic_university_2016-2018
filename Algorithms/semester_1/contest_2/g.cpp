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

const int MAXN = 1e5 + 123;
const int MAXK = 5;
const int MOD = 1000003;
const int MAXLEN = 576;

void mod_add(int &x, int val)
{
    x += val;
    while (x >= MOD)
        x -= MOD;
}

int dp[2][MAXLEN];

vector<int> valid_masks;
vector< vector<int> > connected_masks;
int prev_mask;
int next_mask;

int init_mat[MAXLEN][MAXLEN];
int tmp[MAXLEN][MAXLEN];
int mat_res[MAXLEN][MAXLEN];
int init_vector[MAXLEN];


void mat_pow(int pw, int sz)
{
    while (pw)
    {
        if (pw & 1)
        {
            for (int i = 0; i < sz; i++) 
            {
                for (int j = 0; j < sz; j++)
                {
                    tmp[i][j] = 0;
                    for (int k = 0; k < sz; k++)
                    {
                        long long val = mat_res[i][k];
                        val *= init_mat[k][j];
                        val %= MOD;
                        mod_add(tmp[i][j], val);
                    }
                }
            }
            
            for (int i = 0; i < sz; i++)
                for (int j = 0; j < sz; j++)
                {
                    mat_res[i][j] = tmp[i][j];
                }
        }

        for (int i = 0; i < sz; i++)
        {
            for (int j = 0; j < sz; j++)
            {
                tmp[i][j] = 0;
                for (int k = 0; k < sz; k++)
                {
                    long long val = init_mat[i][k];
                    val *= init_mat[k][j];
                    val %= MOD;
                    mod_add(tmp[i][j], val);
                }       
            }
        }
        for (int i = 0; i < sz; i++)
            for (int j = 0; j < sz; j++)
                init_mat[i][j] = tmp[i][j];
        pw >>= 1;
    }
}


bool is_bad(int mask)
{
    for (int i = 0; i < 25; i++)
    {
        bool bad = true;
        for (int j = 0; j < 3; j++)
        {
            bad &= ((1 << (i + j)) & mask) >> (i + j);
        }

        if (bad) 
        {
            return true;
        }
    }
    return false;
}

void init(int width)
{
    if (width == 0)
        return;
    prev_mask = (1 << width) - 1;
    next_mask = prev_mask << width;
    for (int i = 0; i < (1 << (2 * width)); i++)
    {
        if (is_bad(i & prev_mask))
           continue;
        if (is_bad(i & next_mask))
           continue; 
        valid_masks.push_back(i);
    }

    int now_idx = 0;
    for (const auto& cur: valid_masks)
    {
        connected_masks.push_back(vector<int>());
        for (size_t idx = 0; idx < valid_masks.size(); idx++)
        {
            int prev = valid_masks[idx];
            if ((cur & prev_mask) != ((prev & next_mask) >> width))
                continue;
            bool is_valid = true;
            for (int i = 0; i < width; i++)
            {
                if ((prev & (1 << i)) and (cur & (1 << i)) and (cur & (1 << (i + width))))
                    is_valid = false;
            }
            if (not is_valid)
            {
                continue;
            }
            init_mat[idx][now_idx]++;
            connected_masks.back().push_back(idx);
        }
        now_idx++;
    }
}

int main()
{   
    cin.sync_with_stdio(false);
    int k, n;
    cin >> k >> n;
    if (n == 0)
    {
        cout << 0 << endl;
        return 0;
    }

    init(k);
    
    size_t val_masks = valid_masks.size();
    for (size_t i = 0; i < val_masks; i++)
    {
        int mask = valid_masks[i];
        if (mask & prev_mask)
            continue;
        init_vector[i] = 1;
    }

    for (size_t i = 0; i < val_masks; i++)
        mat_res[i][i] = 1;

    mat_pow(n - 1, val_masks);

    int answer = 0;
    for (size_t i = 0; i < val_masks; i++)
    {
        for (size_t j = 0; j < val_masks; j++)
        {
            mod_add(answer, init_vector[i] * mat_res[i][j]);
        }
    }

    cout << answer << endl;
    return 0;
}
