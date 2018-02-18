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

const int MAXN = 100500;

template<class T>
struct node
{
    T* key;
    unsigned long long priority;
    node* left;
    node* right;

    node(T* key_)
        : key(key_)
        , left(nullptr)
        , right(nullptr)
    {
        priority = rand();
        priority <<= 16;
        priority ^= rand();
        priority <<= 16;
        priority ^= rand();
    }

    void dump()
    {
        if (left)
            left->dump();
        cout << "(" << key->first << ", " << key->second << ") ";
        if (right)
            right->dump();
    }
};

template<class T>
node<T>* merge(node<T>* le, node<T>* rg, bool need_copy=true)
{
    if (le == nullptr and rg == nullptr) return nullptr;
    if (le == nullptr)
    {
        return new node<T> (*rg);
    }
    if (rg == nullptr)
    {
        return new node<T> (*le);
    }

    if (le->priority < rg->priority)
    {
        node<T>* root;
        if(need_copy) root = new node<T> (*rg);
        else root = rg;

        root->left = merge(le, rg->left, need_copy);
        return root;
    }

    node<T>* root;
    if (need_copy) root = new node<T> (*le);
    else root = le;

    root->right = merge(le->right, rg, need_copy);
    return root;
}

template<class T>
pair<node<T>*, node<T>*> split(node<T>* root, T key, bool need_copy=true)
{
    if (root == nullptr) return make_pair(nullptr, nullptr);
    
    node<T>* copy;
    if (need_copy) copy = new node<T> (*root);
    else copy = root;

    if (*(copy->key) < key)
    {
        auto res_rec = split(copy->right, key, need_copy);
        copy->right = nullptr;
        return make_pair(merge(copy, res_rec.first, need_copy), res_rec.second);
    } 
    else 
    {
        auto res_rec = split(copy->left, key, need_copy);
        copy->left = nullptr;
        return make_pair(res_rec.first, merge(res_rec.second, copy, need_copy));
    }
}


using task_j_node = node<pair<int, int>>;

task_j_node* get_max(task_j_node* root)
{
    while (root and root->right)
    {
        root = root->right;
    }

    return root;
}

task_j_node* get_min(task_j_node* root)
{
    while (root and root->left)
    {
        root = root->left;
    }

    return root;
}


bool check_segment_lies_within(task_j_node* root, int beg, int end)
{
    auto key = make_pair(beg + 1, beg + 1);
    auto found_mx = make_pair(-1, -1);

    while (root)
    {
#ifdef DEBUG
        cout << "now at key " << root->key->first << " " << root->key->second << endl;
#endif
        if (*(root->key) >= key)
        {
            root = root->left;
        }
        else
        {
            found_mx = max(found_mx, *(root->key));
            root = root->right;
        }
    }

#ifdef DEBUG
    cout << "found max: " << found_mx.first << " " << found_mx.second << endl;
#endif
    return found_mx.second >= end;
}


task_j_node* add_segment(task_j_node* root, int beg, int end)
{
    // Our new segment can lie within some already added one.
    auto check_lies_within = split(root, make_pair(beg + 1, beg + 1));
    task_j_node* max_less_than_or_equal = get_max(check_lies_within.first);
    if (max_less_than_or_equal != nullptr)
    {
        assert (max_less_than_or_equal->key->first <= beg);
        if (max_less_than_or_equal->key->second >= end)
        {
            return merge(check_lies_within.first, check_lies_within.second, false);
        }
    }

    // Our new segment can cover some already added guys. 
    while (true)
    {
        auto check_covers_someone = split(root, make_pair(beg, beg));
        task_j_node* min_greater_or_equal = get_min(check_covers_someone.second);
        if (min_greater_or_equal == nullptr or min_greater_or_equal->key->second > end)
        {
            root = merge(check_covers_someone.first, check_covers_someone.second, false);
            break;
        }
        else
        {
            pair<int, int> split_key = make_pair(min_greater_or_equal->key->first,
                    min_greater_or_equal->key->second + 1);
            auto split_by_min = split(check_covers_someone.second, split_key);
            delete split_by_min.first;
            root = merge(check_covers_someone.first, split_by_min.second, false);
        }
    } 

    auto added_key = new pair<int, int>(beg, end);
    auto new_node = new task_j_node (added_key);
    auto parts = split(root, *added_key); 

    root = merge(merge(parts.first, new_node), parts.second);
    return root;
}


vector<pair<int, int>> occupied_by_level[MAXN];
task_j_node* roots_by_level[MAXN];

int main()
{   
    cin.sync_with_stdio(false);
    cin.tie(NULL);

    int n, s, m;
    cin >> n >> s >> m;

    for (int i = 0; i < m; i++)
    {
        int place, from, to;
        cin >> place >> from >> to;
        occupied_by_level[place].emplace_back(from, 1);
        occupied_by_level[place].emplace_back(to - 1, -1);
    }

    task_j_node* root = nullptr;

    // Initially, all are free (from 1 to n - 1).
    for (int level = 1; level <= s; level++)
    {
        sort(occupied_by_level[level].begin(),
                occupied_by_level[level].end());
        int last_seen = 0;
        int cur_open = 0;
        occupied_by_level[level].emplace_back(n, 0);

#ifdef DEBUG
        DBG(level);
#endif
        for (const auto &point: occupied_by_level[level])
        {
            int cur_x = point.first;
            if (cur_x - 1 >= last_seen + 1 and cur_open == 0)
            {
#ifdef DEBUG
                cout<< last_seen + 1 << " " << cur_x - 1 << endl;
#endif
                root = add_segment(root, last_seen + 1, cur_x - 1);
            }
            last_seen = cur_x;
            cur_open += point.second;
        }

        roots_by_level[level] = root;
    }

#ifdef DEBUG_TRICKY_TEST
    DBG(check_segment_lies_within(roots_by_level[3], 555555555, 999999991));
    DBG(check_segment_lies_within(roots_by_level[4], 555555555, 999999991));
    DBG(check_segment_lies_within(roots_by_level[5], 555555555, 999999991));
#endif

#ifdef DEBUG_SAMPLE
    roots_by_level[s]->dump();
    cout << endl;
    DBG(check_segment_lies_within(roots_by_level[s], 2, 2));
    return 0;
#endif

    int q;
    int p = 0;
    cin >> q;
    for (int i = 0; i < q; i++)
    {
        int x, y;
        cin >> x >> y;
        int from = x + p;
        int to = y + p;
        if (from > to)
            swap(from, to);

        to--;
        int answer = -1;
        if (not check_segment_lies_within(roots_by_level[s], from, to))
        {
            answer = 0;
        }
        else
        {
            int low = 0, high = s;
            while (high - low > 1)
            {
                int mid = (high + low) / 2;

                if (check_segment_lies_within(roots_by_level[mid],
                            from, to))
                {
                    high = mid;
                }
                else
                {
                    low = mid;
                }
            }

            answer = high;
        }

        cout << answer << "\n";
        p = answer;
    } 

    return 0;
}
