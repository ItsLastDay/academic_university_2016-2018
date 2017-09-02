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
    long long sum;
    unsigned long long priority;
    bool flag;
    node* left;
    node* right;

    void push()
    {
        if(flag)
        {
            flag ^= 1;
            swap(left, right);
            if (left)
                left->flag ^= 1;
            if (right)
                right->flag ^= 1;
        }
    }

    node(int key_)
        : sz(1)
        , key(key_)
        , sum(key_)
        , flag(false)
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
        sum = key;
        sz = 1;
        if (left) 
        {
            sum += left->sum;
            sz += left->sz;
        }
        if (right)
        {
            sum += right->sum;
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
        le->push();
        rg->push();
        res = le;
        le->right = merge(le->right, rg);
    } 
    else 
    {
        le->push();
        rg->push();
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

    root->push();
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
        int height;
        cin >> height;
        node* cur = new node(height);
        root = merge(root, cur);
    }

    for (int i = 0; i < m; i++)
    {
        int q;
        int fr, to;
        cin >> q >> fr >> to;

        auto parts = split(root, to);
        auto p = split(parts.first, fr - 1);
        if (q == 0) 
        {
            cout << p.second->sum << "\n";
        } 
        else
        {
            p.second->flag ^= 1;
        }

        root = merge(p.first, merge(p.second, parts.second));
    }
    return 0;
}
