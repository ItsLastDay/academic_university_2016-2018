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

int get_left(int p)
{
    return 2 * p + 1;
}

int get_right(int p)
{
    return 2 * p + 2;
}

int get_par(int p)
{
    return (p - 1) / 2;
}

int cur_size;
int counter = 0;
int heap[MAXN];

void heapifyDown(int pos)
{
    int largest = pos;

    {
        int child = get_left(pos);
        if(child < cur_size and heap[child] > heap[largest])
            largest = child;
    }
    {
        int child = get_right(pos);
        if(child < cur_size and heap[child] > heap[largest])
            largest = child;
    }

    if(largest == pos)
        return;
    swap(heap[largest], heap[pos]);
    counter++;
    heapifyDown(largest);
}

void heapifyUp(int pos)
{
    while(pos != 0)
    {
        int par = get_par(pos);
        if(heap[par] < heap[pos]) {
            swap(heap[par], heap[pos]);
            pos = par;
        }
        else
            break;
    }
}

void add(int x)
{
    heap[cur_size] = x;
    cur_size++;
    heapifyUp(cur_size - 1);
}

void del()
{
    swap(heap[0], heap[cur_size - 1]);
    cur_size--;
    heapifyDown(0);
}

int main()
{
    cin.sync_with_stdio(false);
    int n;
    cin >> n;
    for(int i = 2; i <= n; i++)
        add(i);
    add(1);

    vector<int> answer(heap, heap + cur_size);

    for(int i = 1; i <= n; i++)
        del();

    cout << counter << "\n";
    for(int i = 0; i < n; i++)
        cout << answer[i] << " ";
    return 0;
}
