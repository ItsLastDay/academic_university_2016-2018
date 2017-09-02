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


const int MAXN = 200500;
const int BLOCK = 450; // sqrt(MAXN)
const int MAX_SIZE = 2 * BLOCK;

list<int> blocks[BLOCK];
int arr[MAXN];
int arr_size;

void build()
{
    for(int i = 0; i < arr_size; i++) {
        blocks[i / BLOCK].push_back(arr[i]);
    }
}

void rebuild()
{
    arr_size = 0;
    for(int i = 0; i < BLOCK; i++) {
        for(const auto &u: blocks[i])
            arr[arr_size++] = u;
        blocks[i].clear();
    }
    build();
}

void del_elem(int idx)
{
    int low = 1;
    for(int i = 0; i < BLOCK; i++) {
        int cur_sz = blocks[i].size();
        if(low + cur_sz - 1 >= idx) {
            auto it = blocks[i].begin();
            advance(it, idx - low);
            blocks[i].erase(it);
            break;
        }
        low += cur_sz;
    }
}

void add_elem(int idx, int val)
{
    if(idx == 0) {
        // before anything
        blocks[0].push_front(val);
        return;
    }
    // after something
    int low = 1;
    for(int i = 0; i < BLOCK; i++) {
        int cur_sz = blocks[i].size();
        if(low + cur_sz - 1 >= idx) {
            auto it = blocks[i].begin();
            advance(it, idx - low + 1);
            blocks[i].insert(it, val);
            if(blocks[i].size() >= MAX_SIZE)
                rebuild();
            break;
        }
        low += cur_sz;
    }
}

int main()
{
    cin.sync_with_stdio(false);
    int m;
    cin >> arr_size >> m;
    for(int i = 0; i < arr_size; i++) {
        cin >> arr[i];
    }
    build();

    for(int i = 0; i < m; i++) {
        string cmd;
        int idx, val;
        cin >> cmd;
        if(cmd == "del") {
            cin >> idx;
            del_elem(idx);
        } else {
            cin >> idx >> val;
            add_elem(idx, val);
        }
    }

    rebuild();

    cout << arr_size << "\n";
    for(int i = 0; i < arr_size; i++)
        cout << arr[i] << " ";
    return 0;
}
