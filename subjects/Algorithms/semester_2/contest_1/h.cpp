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

const int MAXN = 1e5 + 234;

struct node;
pair<node*, int> get_root_from_node(node* );

struct node 
{
    int sz;
    pair<int, int> key;
    unsigned long long priority;
    node* left;
    node* right;
    node* parent;

    node() {}

    node(pair<int, int> key_)
        : sz(1)
        , key(key_)
        , left(nullptr)
        , right(nullptr)
        , parent(nullptr)
    {
        priority = rand();
        priority <<= 16;
        priority ^= rand();
        priority <<= 16;
        priority ^= rand();
    }

    void comp()
    {
        parent = nullptr;
        sz = 1;
        if (left) 
        {
            sz += left->sz;
            left->parent = this;
        }
        if (right)
        {
            sz += right->sz;
            right->parent = this;
        }
    }

    void dfs(node* pp=nullptr)
    {
        assert(pp == parent);
        if (left)
        {
            //cout << "turning left" << endl;
            left->dfs(this);
            //cout << "going top" << endl;
        }
        auto pos = get_root_from_node(this).second;
        cout << "key: (" << key.first << ", " << key.second << ")" << " ";//<< " on pos "
//            << pos << endl;
        if (right)
        {
//            cout << "turning right" << endl;
            right->dfs(this);
//            cout << "going top" << endl;
        }
    }
};

node pool[MAXN * 4];
int pool_free = 0;

node* get_node(pair<int, int> key)
{
    auto res = new (pool + pool_free) node(key);
    ++pool_free;
    return res;
}


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
    if (root == nullptr) return make_pair(root, root);
    root->comp();
    if (sz == 0) return make_pair(nullptr, root);

    int sz_left = (root->left == nullptr ? 0 : root->left->sz);

    if (sz_left >= sz)
    {
        auto res = split(root->left, sz);
        root->left = nullptr;
        root = merge(res.second, root);

        if (res.first)
            res.first->comp();
        if (root)
            root->comp();
        return make_pair(res.first, root);
    }

    sz -= sz_left + 1;
    auto rg = split(root->right, sz);
    root->right = nullptr;
    root = merge(root, rg.first);

    if (root)
        root->comp();
    if (rg.second)
        rg.second->comp();

    return make_pair(root, rg.second);
}


// i -> node of (i, i)
node* idx_to_node[MAXN];

pair<node*, int> get_root_from_node(node* root)
{
    int pos_in_root = 1 + (root->left ? root->left->sz : 0);
    node* nxt;
    int it = 0;
    while (root->parent != nullptr)
    {
        ++it;
        assert(it < 1e6);
        nxt = root->parent;
        if (nxt->right == root)
        {
            pos_in_root += 1 + (nxt->left ? nxt->left->sz : 0);
        }
        root = nxt;
    }

    return make_pair(root, pos_in_root);
}


pair<node*, int> get_root_idx(node* root)
{
    root = get_root_from_node(root).first;
    auto p = split(root, root->sz - 1);
    int res = p.second->key.second;
    root = merge(p.first, p.second);
    return make_pair(root, res);
}

node* tree_rerooted_with(int new_root_idx)
{
    auto p_root = get_root_from_node(idx_to_node[new_root_idx]);

    auto p = get_root_idx(p_root.first);
    if (p.second == new_root_idx)
        return p.first;

    auto parts = split(p_root.first, p_root.second);
    return merge(parts.second, parts.first);
}

map< pair<int, int>, node*> edge_to_node;

node* create_edge(int fr, int to)
{
    auto key = make_pair(fr, to);
    auto nd = get_node(key);
    edge_to_node[key] = nd;
    return nd;
}


int main()
{   
    cin.sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    cin >> n >> m;
    for (int i = 1; i <= n; i++) 
    {
        auto cur = get_node(make_pair(i, i));
        idx_to_node[i] = cur;
    }

    for (int i = 0; i < m; i++) 
    {
        string cmd;
        int fr, to;
        cin >> cmd >> fr >> to;
        if (cmd == "link")
        {
            auto root_to = tree_rerooted_with(to);
            auto new_node_a = create_edge(fr, to);
            auto new_node_b = create_edge(to, fr);
            auto root_from = get_root_from_node(idx_to_node[fr]);

            auto fr_before_after = split(root_from.first, root_from.second);
            
            auto xx = merge(fr_before_after.first,
                    merge(new_node_a, 
                        merge(root_to, 
                            merge(new_node_b, fr_before_after.second))));

            (void)xx;
#ifdef DEBUG
            cout << endl;
            xx->dfs();
            cout << endl;
#endif
        }
        else if (cmd == "cut")
        {
            auto place1 = get_root_from_node(edge_to_node[make_pair(fr, to)]);
            auto place2 = get_root_from_node(edge_to_node[make_pair(to, fr)]);

            if (place1.second > place2.second)
            {
                swap(place1, place2);
            }

#ifdef DEBUG
            DBG(place1.second);
            DBG(place2.second);
#endif

            auto r_before = split(place2.first, place2.second);
            auto r_after_second = r_before.second;

            auto deleted_second = split(r_before.first, place2.second - 1).first;

            auto to_before = split(deleted_second, place1.second);
            auto deleted_first = split(to_before.first, place1.second - 1).first;

            merge(deleted_first, r_after_second);

#ifdef DEBUG
            cout << "DUMPING STATE AFTER CUT " << fr << " " << to << endl;
            for (int j = 1; j <= n; j++)
            {
                auto p = get_root_from_node(idx_to_node[j]);
                cout << j << ": ";
                p.first->dfs();
                cout << endl;
            }
            cout << endl;
#endif
        }
        else
        {
            auto r_from = get_root_from_node(idx_to_node[fr]);
            auto r_to = get_root_from_node(idx_to_node[to]);

#ifdef DEBUG
            cout << endl;
            r_from.first->dfs();
            cout << endl;
#endif
            cout << (r_from.first == r_to.first ? '1' : '0');
        }
    }
    return 0;
}
