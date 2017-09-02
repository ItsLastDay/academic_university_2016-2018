#include "letter_it.h"

#include <iostream>

#include <list>
#include <vector>
#include <set>
#include <unordered_set>

#include <iterator>
#include <cctype>


template<class fwd_it>
void print(fwd_it begin, fwd_it end)
{
    while (begin != end)
    {
        std::cout << *begin << " ";
        ++begin;
    }
    std::cout << std::endl;
}


template<class fwd_it, 
    class comparator=std::less<typename std::iterator_traits<fwd_it>::value_type>>
void tree_sort(fwd_it begin, fwd_it end, comparator cmp=comparator())
{
    std::multiset<typename std::iterator_traits<fwd_it>::value_type, comparator> sorting_container(begin, end, cmp);
    std::copy(sorting_container.begin(), sorting_container.end(), begin);
}


std::string tolower(std::string const& a)
{
    std::string lowercase_a = a;
    for (size_t i = 0; i < a.size(); ++i)
    {
        lowercase_a[i] = tolower(lowercase_a[i]);
    }
    return lowercase_a;
}


struct case_insensitive_less
{
    bool operator()(std::string const& a, std::string const& b)
    {
        return tolower(a) < tolower(b);
    }
};


struct modular_comparator
{
    modular_comparator() = delete;
    modular_comparator(unsigned mod): mod_(mod) {}

    bool operator()(const int &a, const int &b)
    {
        return a % mod_ < b % mod_;
    }

private:
    unsigned mod_;
};


template<class T,
         class Hash=std::hash<T>,
         class Pred=std::equal_to<T>>
void remove_duplicates(std::list<T> &lst, 
                       Hash hash=Hash(), 
                       Pred eq=Pred())
{
    std::unordered_set<T, Hash, Pred> seen_elems(1, hash, eq);
    for (auto it = lst.begin(); it != lst.end(); )
    {
        // Can decrease constant factor by looking at 
        // the second element of "find" return value.
        if (seen_elems.count(*it))
        {
            it = lst.erase(it);
        }
        else
        {
            seen_elems.insert(*it);
            ++it;
        }
    }
}


int main()
{
    std::vector<int> v = {1,2,3, -5, 1, 4, 100};
    print(v.begin(), v.end());
    // Can omit type of modular_comparator in template, because it can
    // be deduced.
    tree_sort(v.begin(), v.end(), modular_comparator(2));
    print(v.begin(), v.end());

    std::vector<std::string> vs = {"ab", "", "cddsfWER"};
    tree_sort(vs.begin(), vs.end());
    print(vs.begin(), vs.end());

    std::string arr[5] = {"AbC", "abc", "qqq", "tr", "Tz"};
    tree_sort(arr, arr + 5);
    print(arr, arr + 5);
    tree_sort<std::string*, case_insensitive_less>(arr, arr + 5);
    print(arr, arr + 5);


    std::list<int> lst = { 1, 1, 2, 3, 4, 5, 6, 5, 7, 5, 8, 9 };
    remove_duplicates(lst);
    print(lst.begin(), lst.end());

    std::list<int> lst_2 = { 1, 1, 2, 3, 4, 5, 6, 5, 7, 5, 8, 9 };
    auto hasher = [](int const& x) -> bool
    {
        return x - 123; 
    };
    auto eq = [](int const& x, int const &y) -> bool
    {
        return (x & 1) == (y & 1);
    };
    remove_duplicates(lst, hasher, eq);
    print(lst.begin(), lst.end());
    return 0;
}
