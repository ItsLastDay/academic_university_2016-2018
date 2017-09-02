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

const int MAXN = 800500;

int trie_go[MAXN][2];
char trie_mark[MAXN];
int next_free = 1;

void add_trie(string const& s, char symb)
{
    int cur_node = 0;
    for (const auto &c: s)
    {
        int idx = c - '0';
        if (trie_go[cur_node][idx] == 0)
            trie_go[cur_node][idx] = next_free++;
        cur_node = trie_go[cur_node][idx];
    }

    trie_mark[cur_node] = symb;
}

int main()
{   
    cin.sync_with_stdio(false);
    cin.tie(NULL);

    int num_symbols, n;
    cin >> num_symbols >> n;
    for (int i = 0; i < num_symbols; i++)
    {
        string symbol;
        cin >> symbol;
        string pattern;
        cin >> pattern;

        add_trie(pattern, symbol[0]);
    }

    string text;
    cin >> text;
    int cur_node = 0;
    for (int i = 0; i < n; i++)
    {
        int idx = text[i] - '0';
        cur_node = trie_go[cur_node][idx];
        if (trie_mark[cur_node])
        {
            cout << trie_mark[cur_node];
            cur_node = 0;
        }
    }
    return 0;
}
