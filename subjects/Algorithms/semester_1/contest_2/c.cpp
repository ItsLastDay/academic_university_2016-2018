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

const int MAXN = 100500;

int arr[MAXN];

struct tree_node
{
    int le, rg;
    int val;
    tree_node *left, *right;

    tree_node(int le_, int rg_, int val_): le(le_), rg(rg_), val(val_) 
    {
        left = nullptr;
        right = nullptr;
    }
};

tree_node* build(int le, int rg)
{
    if (le == rg)
    {
        return nullptr;
    }
    auto x = new tree_node(le, rg, 0);
    x->left = build(le, (le + rg) / 2);
    x->right = build((le + rg) / 2 + 1, rg);
    if (x->left)
        x->val += x->left->val;
    else
        x->val += arr[le];

    if (x->right)
        x->val += x->right->val;
    else
        x->val += arr[rg];

    return x;
}

void add(tree_node* node, int idx, int val)
{
    if (node == nullptr)
    {
        arr[idx] += val;
        return;
    }

    if (idx < node->le or idx > node->rg)
        return;

    node->val += val;

    if (node->left != nullptr or node->le == idx)
        add(node->left, idx, val);
    if (node->right != nullptr or node->rg == idx)
        add(node->right, idx, val);
}

int get_sum(tree_node* node, int L, int R)
{
    if(L <= node->le and node->rg <= R)
        return node->val;
    if(L > node->rg or R < node->le)
        return 0;

    int ans = 0;
    if (node->left == nullptr)
    {
        if (L <= node->le and node->le <= R)
            ans += arr[node->le];
    }
    else
        ans += get_sum(node->left, L, R);

    if(node->right == nullptr)
    {
        if (L <= node->rg and node->rg <= R)
            ans += arr[node->rg];
    }
    else
        ans += get_sum(node->right, L, R);

    return ans;
}

int main()
{   
    cin.sync_with_stdio(false);
    cin.tie(NULL);
    int n;
    cin >> n;
    for (int i = 0; i < n; i++)
        cin >> arr[i];

    auto root = build(0, n - 1);

    int q;
    cin >> q;
    for (int i = 0; i < q; i++)
    {
        string cmd;
        cin >> cmd;
        if (cmd == "count")
        {
            int le, rg;
            cin >> le >> rg;
            le--; rg--;
            int answer = 0;
            if (le <= rg)
            {
                answer = get_sum(root, le, rg);
            }
            else
            {
                answer = get_sum(root, le, n - 1);
                answer += get_sum(root, 0, rg);
            }
            cout << answer << "\n";
        }
        else
        {
            int from, to, amount;
            cin >> from >> to >> amount;
            from--;
            to--;
            add(root, from, -amount);
            add(root, to, amount);
        }
    }
    return 0;
}
