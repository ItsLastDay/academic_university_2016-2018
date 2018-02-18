#include <bits/stdc++.h>
using namespace std;
using namespace std::chrono;
#define DBG(x) cout << #x << " = " << x << endl;
/*
Compile:
    -Wall -Wextra -pedantic -std=c++14 -O2 -Wshadow -Wformat=2 -Wfloat-equal -Wconversion -Wlogical-op -Wcast-qual -Wcast-align -fwhole-program -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -lmcheck -fsanitize=address 

    -std=c++11 -O2
 
Check for memory usage:
    valgrind --tool=massif
*/

const int MAXN = 100500;
const long double EPS = 1e-11;

using ll = long double;

enum
{
    CHECK, DELETE, ADD
};

vector<unsigned long long> priorities(MAXN);
int prior_free_idx = 0;

template<class T>
struct node
{
    T key;
    unsigned long long priority;
    int sz;
    node* left;
    node* right;
    node* parent;

    node() = default;

    node(T key_)
        : key(key_)
        , sz(1)
        , left(nullptr)
        , right(nullptr)
    {
        priority = priorities[prior_free_idx++];
    }

    void calc()
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
};

template<class T>
node<T>* merge(node<T>* le, node<T>* rg)
{
    if (le == nullptr and rg == nullptr) return nullptr;
    node<T>* res = nullptr;

    if (le == nullptr)
    {
        res = rg;
    }
    else if (rg == nullptr)
    {
        res = le;
    }
    else if (le->priority < rg->priority)
    {
        res = rg;
        res->left = merge(le, rg->left);
    } 
    else 
    {
        res = le;
        res->right = merge(le->right, rg);
    }

    if (res)
        res->calc();

    return res;
}


struct segment
{
    ll k, b;

    segment() = default;

    segment(int x1, int y1, int x2, int y2)
    {
        k  = static_cast<ll>(y1 - y2) / (x1 - x2);
        b = y1 - k * x1;
    }

    ll compute_key(ll x)
    {
        return k * x + b;
    }
};

using task_i_node = node<segment>;


pair<task_i_node*, task_i_node*> split(task_i_node* root, ll x, ll y,
        segment const& added_segm)
{
    if (root == nullptr) return make_pair(nullptr, nullptr);
    root->calc();
    
    auto rk = root->key.compute_key(x);
    if (rk < y - EPS or (abs(rk - y) <= EPS and root->key.k < added_segm.k))
    {
        auto res_rec = split(root->right, x, y, added_segm);
        root->right = nullptr;
        root = merge(root, res_rec.first);
        if (root)
            root->calc();
        return make_pair(root, res_rec.second);
    } 
    else 
    {
        auto res_rec = split(root->left, x, y, added_segm);
        root->left = nullptr;
        root = merge(res_rec.second, root);
        if (root)
            root->calc();
        return make_pair(res_rec.first, root);
    }
}


pair<task_i_node*, int> find_and_count(task_i_node* root, ll x, ll y)
{
    int pos = 0;
    task_i_node* last_found = nullptr;

    while (root)
    {
        ll seg_pos = root->key.compute_key(x);

        if (seg_pos > y + EPS)
        {
            root = root->left;
        }
        else
        {
            last_found = root;
            pos += 1 + (root->left ? root->left->sz : 0);
            root = root->right;
        }
    }

    return make_pair(last_found, pos);
}

vector<segment> segments(MAXN);

vector<task_i_node> nodes(MAXN);
int free_node_idx = 0;

vector< tuple<int, int, int, int> > events(3 * MAXN);
int ev_size;
vector< pair<int, int> > points(MAXN);
set< pair<int, int> > rect_points;
unordered_map<int, set< pair<int, int>>> vertical_segs;

const char* answers[MAXN];

const char* BORDER = "BORDER\n";
const char* INSIDE = "INSIDE\n";
const char* OUTSIDE = "OUTSIDE\n";

void solve()
{
    free_node_idx = 0;
    ev_size = 0;
    prior_free_idx = 0;
    rect_points.clear();
    vertical_segs.clear();
    int n;
    int q;
    cin >> n;

    for (int i = 0; i < n; i++)
    {
        cin >> points[i].first >> points[i].second;
        rect_points.insert(points[i]);
    }

    for (int i = 0; i < n; i++)
    {
        int x1 = points[i].first;
        int y1 = points[i].second;
        int next_idx = i + 1;
        if (next_idx == n)
            next_idx = 0;
        int x2 = points[next_idx].first;
        int y2 = points[next_idx].second;

        if (x1 == x2)
        {
            vertical_segs[x1].insert(make_pair(min(y1, y2), max(y1, y2)));
            continue;
        }
        else if (x1 > x2)
        {
            swap(x1, x2);
            swap(y1, y2);
        }

        segments[i] = segment(x1, y1, x2, y2);
        events[ev_size++] = make_tuple(x1, ADD, y1, i);
        events[ev_size++] = make_tuple(x2, DELETE, y2, i);
    }

    cin >> q;

    for (int i = 0; i < q; i++)
    {
        int x, y;
        cin >> x >> y;
        events[ev_size++] = make_tuple(x, CHECK, y, i);
    }

    sort(events.begin(), events.begin() + ev_size);

    vector<task_i_node*> seg_to_node(n);

    task_i_node* root = nullptr;
    for (int _it = 0; _it < ev_size; _it++)
    {
        auto& u = events[_it];

        int x, y, idx;
        int type;

        tie(x, type, y, idx) = u;

        if (type == CHECK)
        {
            if (rect_points.count(make_pair(x, y)))
            {
                answers[idx] = BORDER;
                continue;
            }
            auto check_vert = vertical_segs[x].lower_bound(make_pair(y + 1, y + 1));
            if (check_vert != vertical_segs[x].begin())
            {
                check_vert = prev(check_vert);
                if (check_vert->second >= y)
                {
                    answers[idx] = BORDER;
                    continue;
                }
            }

            auto fnd = find_and_count(root, x, y);
            if (fnd.first != nullptr and abs(fnd.first->key.compute_key(x) - y) <= EPS)
            {
                answers[idx] = BORDER;
                continue;
            }

            answers[idx] = fnd.second & 1 ? INSIDE : OUTSIDE;
        }
        else if (type == ADD)
        {
            segment& seg = segments[idx];
            auto node_to_add = new (&nodes[free_node_idx++]) task_i_node(seg);
            seg_to_node[idx] = node_to_add;

            auto splitted_tree = split(root, x, y, seg);
            root = merge(merge(splitted_tree.first, node_to_add), splitted_tree.second);
        }
        else if (type == DELETE)
        {
            auto fnd = seg_to_node[idx]; 

            if (fnd->parent == nullptr)
            {
                root = merge(root->left, root->right);
                continue;
            }


            auto par = fnd->parent;
            if (fnd->parent->left == fnd)
            {
                auto pp = merge(fnd->left, fnd->right);
                par->left = pp;
            }
            else if (fnd->parent->right == fnd)
            {
                auto pp = merge(fnd->left, fnd->right);
                par->right = pp;
            } 
            else exit(123);

            while (par)
            {
                auto par_nxt = par->parent;
                par->calc();
                par = par_nxt;
            }
        }
        else exit(666);
    }

    for (int i = 0; i < q; i++)
    {
        cout << answers[i];
    }
}


int main()
{   
    cin.sync_with_stdio(false);
    cin.tie(NULL);

    for (int i = 0; i < MAXN; i++)
    {
        unsigned long long priority;
        priority = rand();
        priority <<= 16;
        priority ^= rand();
        priority <<= 16;
        priority ^= rand();
        priorities[i] = priority;
    }

    int t;
    cin >> t;
    for (int i = 0; i < t; i++)
        solve();
    return 0;
}
