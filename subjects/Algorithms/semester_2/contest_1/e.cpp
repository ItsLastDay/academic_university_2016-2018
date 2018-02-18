#include <bits/stdc++.h>
using namespace std;
using namespace std::chrono;
#define DBG(x) cout << #x << " = " << x << "\n";
/*
Compile:
    -Wall -Wextra -pedantic -std=c++14 -O2 -Wshadow -Wformat=2 -Wfloat-equal -Wconversion -Wlogical-op -Wcast-qual -Wcast-align -fwhole-program -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -lmcheck -fsanitize=address 

    -std=c++11 -O2
 
Check for memory usage:
    valgrind --tool=massif
*/

struct node 
{
    int sz;
    int key;
    int mn;
    unsigned long long priority;
    node* left;
    node* right;

    node(int key_)
        : sz(1)
        , key(key_)
        , mn(key_)
        , left(nullptr)
        , right(nullptr)
    {
        priority = rand();
        priority <<= 16;
        priority ^= rand();
        priority <<= 16;
        priority ^= rand();
    }

    void comp()
    {
        mn = key;
        sz = 1;
        if (left) 
        {
            mn = min(mn, left->mn);
            sz += left->sz;
        }
        if (right)
        {
            mn = min(mn, right->mn);
            sz += right->sz;
        }
    }

    void dfs()
    {
        if (left)
            left->dfs();
        cout << key << " ";
        if (right)
            right->dfs();
    }
};


node* merge(node* le, node* rg)
{
    node* res = nullptr;
    if (le == nullptr) res = rg;
    else 
    if (rg == nullptr) res = le;
    else
    if (le->priority >= rg->priority)
    {
        res = le;
        le->right = merge(le->right, rg);
    } 
    else 
    {
        res = rg;
        rg->left = merge(le, rg->left);
    }

    if (res)
        res->comp();
    return res;
}


pair<node*, node*> split(node* root, int sz)
{
    // <result, everything else>
    if (sz == 0) return make_pair(nullptr, root);
    if (root == nullptr) return make_pair(root, root);

    int sz_left = (root->left == nullptr ? 0 : root->left->sz);

#ifdef DEBUG
    DBG(sz_left);
    DBG(sz);
    DBG(root->sz);
#endif
    if (sz_left >= sz)
    {
        auto res = split(root->left, sz);
        root->left = nullptr;
        root = merge(res.second, root);
        return make_pair(res.first, root);
    }

    sz -= sz_left + 1;
    auto rg = split(root->right, sz);
    root->right = nullptr;
    return make_pair(merge(root, rg.first), rg.second);
}


int main()
{   
    cin.sync_with_stdio(false);
    cin.tie(NULL);

    node* root = nullptr;
    int n, m;
    cin >> n >> m;
    for (int i = 1; i <= n; i++) 
    {
        node* cur = new node(i);
        root = merge(root, cur);
    }

    for (int i = 0; i < m; i++)
    {
        int fr, to;
        cin >> fr >> to;

        auto parts = split(root, to);
        auto p = split(parts.first, fr - 1);

        root = merge(p.second, merge(p.first, parts.second));
    }

    root->dfs();
    return 0;
}