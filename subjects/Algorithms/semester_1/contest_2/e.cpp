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
        return new tree_node(le, rg, arr[le]);
    auto x = new tree_node(le, rg, 0);
    x->left = build(le, (le + rg) / 2);
    x->right = build((le + rg) / 2 + 1, rg);
    x->val = x->left->val + x->right->val;
    return x;
}

tree_node* add(tree_node* node, int idx, int val)
{
    if (idx < node->le or idx > node->rg)
        return node;
    tree_node* copy_node = new tree_node(node->le, node->rg, node->val);
    copy_node->val += val;
    if (node->le == node->rg)
        return copy_node;
    copy_node->left = add(node->left, idx, val);
    copy_node->right = add(node->right, idx, val);

    return copy_node;
}

int get_sum(tree_node* node, int L, int R)
{
    if(L <= node->le and node->rg <= R)
        return node->val;
    if(L > node->rg or R < node->le)
        return 0;
    return get_sum(node->left, L, R) + get_sum(node->right, L, R);
}


vector< pair<int, tree_node*> > versions;

tree_node* get_version(int x)
{
    int le = 0, rg = static_cast<int>(versions.size());
    while (rg - le > 1)
    {
        int mid = (le + rg) / 2;
        if (versions[mid].first < x)
            le = mid;
        else
            rg = mid;
    }

//    cout << x << " " << versions[le].first << endl;
    return versions[le].second;
}

int get_answer(int x1, int y1, int x2, int y2)
{
    auto vhigh = get_version(x2 + 1);
    auto vlow = get_version(x1);

    return get_sum(vhigh, y1, y2) - get_sum(vlow, y1, y2);
}

int main()
{   
    cin.sync_with_stdio(false);
    cin.tie(NULL);

    int w, h;
    int n;
    cin >> w >> h;

    vector< pair<int, int> > stars;

    cin >> n;
    for (int i = 0; i < n; i++)
    {
        int x, y;
        cin >> x >> y;
        stars.push_back(make_pair(x, y));
    }
    sort(stars.begin(), stars.end());

    auto root = build(0, 100000);
    versions.push_back(make_pair(-1, root));

    for (int i = 0; i < n; i++)
    {
        int j = i;
        while (j < n and stars[j].first == stars[i].first)
            j++;

        for (int k = i; k < j; k++)
        {
            root = add(root, stars[k].second, 1);
        }

        versions.push_back(make_pair(stars[i].first, root));

        i = j - 1;
    }

    /*
    for (const auto &p: versions)
    {
        cout << p.first << " " << p.second << endl;
    }
    */
//    cout << get_sum(versions[2].second, 0, 1) << endl;


    int q;
    cin >> q;
    int xlp, ylp, xhp, yhp;
    xlp = w / 2;
    ylp = h / 2;
    xhp = w;
    yhp = h;
    int answer;
    for (int i = 0; i < q; i++)
    {
        if (i == 0)
        {
            answer = get_answer(xlp, ylp, xhp, yhp);
        }
        else
        {
            int x1, x2, y1, y2;
            x1 = ((xlp + answer) * 13 + 7) % (w + 1);
            x2 = ((xhp + answer) * 7 + 13) % (w + 1);
            y1 = ((ylp + answer) * 13 + 7) % (h + 1);
            y2 = ((yhp + answer) * 7 + 13) % (h + 1);
            xlp = min(x1, x2);
            xhp = max(x1, x2);
            ylp = min(y1, y2);
            yhp = max(y1, y2);
            answer = get_answer(xlp, ylp, xhp, yhp);
        }
        cout << answer << "\n";
    }
    return 0;
}
