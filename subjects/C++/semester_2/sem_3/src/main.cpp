#include "my_algorithms.h"

#include <iostream>
#include <cassert>

#include <forward_list>
#include <deque>
#include <functional>

using namespace std;

void test_elem_in_num_sorted()
{
    int sorted_ar[] = {1, 2, 3, 4, 5, 6, 7, 7, 7, 8, 9 , 10, 11};
    assert(elem_num_in_sorted(sorted_ar, sorted_ar + 13, 7) == 3);

    forward_list<int> lst = {1, 51, 163, 3, 0, 16};
    auto eq_modulo_2 = [](int x, int y) -> bool { return x % 2 > y % 2; };
    assert(elem_num_in_sorted(lst.begin(), lst.end(), 17, eq_modulo_2) == 4);  
    assert(elem_num_in_sorted(lst.begin(), lst.end(), 6, eq_modulo_2) == 2);  
    auto lst_beg = lst.begin();
    advance(lst_beg, 2);
    assert(elem_num_in_sorted(lst_beg, lst.end(), 1, eq_modulo_2) == 2);

// The following should not compile when `comparator cmp=std::less<T>()` is written in 
// function definition.
//    size_t functor_res = elem_num_in_sorted<forward_list<int>::iterator,
//          int, modulus<int>>(lst.begin(), lst.end(), 0, ); 
//    assert(functor_res == 4);
}


void test_set_add()
{
    vector<int> set;
    assert(*set_add(set, 10) == 10);
    assert(set_add(set, 10) == set.end());
    assert(*set_add(set, 5) == 5);
    assert((set[0] == 5) && (set[1] == 10));
}


void test_erase_if()
{
    vector<int> set = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    erase_if(set, [](int val) { return val > 6; });
    assert(set.size() == 6);
    for (int i = 1; i <= 6; i++)
        assert(set[i - 1] == i);
}


void test_merge_sort()
{
    std::vector<int> vec = {
               10, 9, 4, 345, 54, 2, 4, 12
                              , 4, 2, 34, 34, 56, 4, 67
                                         , 3465, 3, 4, 5, 34, 5
                                            };

    merge_sort(vec.begin(), vec.end());
    assert(is_sorted(vec.begin(), vec.end()));

    forward_list<int> lst = {5, 3, 1, 123, 56};
    merge_sort(lst.begin(), lst.end());
    assert(is_sorted(lst.begin(), lst.end()));

    deque<int> d;
    merge_sort(d.begin(), d.end());
    assert(is_sorted(d.begin(), d.end()));
}


void test_heap_sort()
{
    std::vector<int> vec = {
               10, 9, 4, 345, 54, 2, 4, 12
                              , 4, 2, 34, 34, 56, 4, 67
                                         , 3465, 3, 4, 5, 34, 5
                                            };

    heap_sort(vec.begin(), vec.end());
    assert(is_sorted(vec.begin(), vec.end()));

    std::vector<int> empty;
    heap_sort(empty.begin(), empty.end());
}


void test_inputed_filter()
{
    std::vector<std::string> vec = {
            "1", "2", "3", "4", "5", "6", "7", "8", "9", "10"
    };
    inputed_filter<std::string> filter;
    auto filter_ref = std::ref(filter);
    erase_if(vec, filter_ref);

    for (size_t i = 0; i < vec.size(); i++)
        cout << vec[i] << " ";
    cout << endl;
}


int main()
{
    test_elem_in_num_sorted();
    test_set_add();
    test_erase_if();
    test_merge_sort();
    test_heap_sort();
//  Don't prompt user input on each test.
//    test_inputed_filter();
    return 0;
}
