#include <bits/stdc++.h>
using namespace std;

random_device rd;
mt19937 twister(rd());

template<class T>
class BaseHeap
{
public:
    virtual ~BaseHeap() {};
    virtual T const& peek_max() const = 0;
    virtual T extract_max() = 0;
    virtual void insert(T const& p) = 0;
    virtual void debug_print(size_t, int) = 0;
};



template<class T>
class ArrayHeap
    : public BaseHeap<T>
{
    unsigned const ROOT = 0;

    vector<T> tree;

    size_t get_left(size_t idx) const
    {
        return idx * 2 + 1;
    }

    size_t get_right(size_t idx) const
    {
        return idx * 2 + 2;
    }

    size_t get_parent(size_t idx) const 
    {
        if (idx == ROOT)
        {
            return ROOT;
        }
        return (idx - 1) / 2;
    }

    void promote(size_t idx)
    {
        while (idx != ROOT)
        {
            auto parent = get_parent(idx);
            if (tree[parent] <= tree[idx])
            {
                swap(tree[parent], tree[idx]);
                idx = parent;
            } 
            else
            {
                break;
            }
        }
    }

    void heapify(size_t idx)
    {
        // Merge two valid heaps (rooted at `left` and `right`)
        // into one with root in `idx`.
        // Non-recursive.
        while (true)
        {
            auto left = get_left(idx);
            auto right = get_right(idx);

            auto largest = idx;

            if (left < tree.size() and tree[left] > tree[largest])
            {
                largest = left;
            }

            if (right < tree.size() and tree[right] > tree[largest])
            {
                largest = right;
            }

            if (largest == idx)
            {
                break;
            }

            swap(tree[largest], tree[idx]);
            idx = largest;
        }
    }

    void make_heap()
    {
        // O(n) heap construction (because sum of h/2^h is bounded).
        size_t sz = tree.size();
        if (sz == 0)
        {
            return;
        }

        for (int i = get_parent(sz - 1); i >= static_cast<int>(ROOT); i--) 
        {
            heapify(i);
        }
    }

public:
    ArrayHeap<T>(vector<T> const &arr)
    {
        tree = arr;
        make_heap();
    }

    T const& peek_max() const
    {
        return tree.front();
    }

    T extract_max()
    {
        T result = tree[ROOT];
        swap(tree[ROOT], tree.back());
        tree.pop_back();
        heapify(ROOT);
        return result;
    }

    void insert(T const &key)
    {
        tree.push_back(key);
        promote(tree.size() - 1);
    }

    ~ArrayHeap() {}

    void debug_print(size_t idx, int spaces)
    {
        if (idx >= tree.size())
        {
            return;
        }

        cout << string(spaces, ' ');
        //cout << tree[idx] << endl;
        debug_print(get_left(idx), spaces + 2);
        debug_print(get_right(idx), spaces + 2);
    }
};

template<class T>
bool test_heapsort(vector<T> const& data)
{
    multiset<T> st;
    for (auto const& u: data)
    {
        st.insert(u);
    }

    auto hh = ArrayHeap<T>(data);
    BaseHeap<T> &heap = hh;

    while (not st.empty())
    {
        for (int i = 0; i < 2; i++) 
        {
            //heap.debug_print(0, 0);
            auto heap_result = heap.extract_max();
            auto st_result = *(st.rbegin());
            st.erase(--st.end());

            //cout << heap_result << endl << st_result << endl;
            if (heap_result != st_result)
            {
                return false;
            }

            if (i == 0)
            {
                heap.insert(heap_result);
                st.insert(heap_result);
            }
        }
    }

    return true;
}

vector<int> get_data()
{
    vector<int> ret;
    
    auto size_chooser = uniform_int_distribution<int>(0, 1000);
    int sz = size_chooser(twister);

    auto val_chooser = uniform_int_distribution<int>(-100, 100);

    for (int i = 0; i < sz; i++)
    {
        ret.push_back(val_chooser(twister));
    }

    return ret;
}

vector< pair<int, int> > get_data_pairs()
{
    vector< pair<int, int> > ret;
    
    auto size_chooser = uniform_int_distribution<int>(0, 1000);
    int sz = size_chooser(twister);

    auto val_chooser = uniform_int_distribution<int>(-100, 400);

    for (int i = 0; i < sz; i++)
    {
        ret.push_back(make_pair(val_chooser(twister),
                    val_chooser(twister)));
    }

    return ret;
}

int main()
{
    while (true)
    {
        auto data = get_data_pairs();
        bool is_good = test_heapsort(data);

        if (not is_good)
        {
            cout << "Something is wrong!" << endl;
            for (auto const &u: data)
            {
                //cout << u << ", ";
                if (u.first) {}
            }
            break;
        }

        cout << "OK " << data.size() << endl;
    }
    return 0;
}
