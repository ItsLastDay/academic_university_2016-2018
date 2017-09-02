#pragma once
#include <iostream>

#include <algorithm>
#include <iterator>
#include <functional>

#include <array>


template<class fwd_it, class T, class comparator=std::less<T>>
typename std::iterator_traits<fwd_it>::difference_type 
elem_num_in_sorted(fwd_it beg, fwd_it end, T elem, 
     comparator comp=comparator())
{
    auto equal_beg_end_pair = std::equal_range(beg, end, elem, comp);
    return std::distance(equal_beg_end_pair.first, equal_beg_end_pair.second);
}


template<class container>
typename container::iterator
set_add(container& cont, typename container::const_reference elem)
{
    // `cont` is sorted, we need to maintain sortedness after insert.
    auto it = lower_bound(cont.begin(), cont.end(), elem);
    if (it == cont.end() or *it != elem)
    {
        return cont.insert(it, elem);
    }

    return cont.end();
}


template<class container, class pred>
void 
erase_if(container& cont, pred predicate=pred())
{
    auto beg_removed = std::remove_if(cont.begin(), cont.end(), predicate);
    cont.erase(beg_removed, cont.end());
} 


template<class fwd_it>
void 
merge_sort(fwd_it begin, fwd_it end)
{
    auto len = std::distance(begin, end);
    if (len <= 1)
    {
        return;
    }

    auto begin_sec_half = begin;
    std::advance(begin_sec_half, len / 2);

    merge_sort(begin, begin_sec_half);
    merge_sort(begin_sec_half, end);

    std::merge(begin, begin_sec_half,
            begin_sec_half, end,
            begin);
}


template<class ran_it>
void
heap_sort(ran_it begin, ran_it end)
{
    std::make_heap(begin, end);

    while (end != begin) 
    {
        std::pop_heap(begin, end);
        --end;
    }
}


template<class T>
struct inputed_filter
    : std::unary_function<T, bool>
{
    inputed_filter()
        : is_user_queried_(false)
    {
    }

    bool operator()(T const& elem)
    {
        if (not is_user_queried_)
        {
            is_user_queried_ = true;
            query_filtered_values();
        }

        return std::find(filtered_values_.begin(),
                filtered_values_.end(),
                elem) != filtered_values_.end();
    }

private:
    void query_filtered_values()
    {
        std::copy_n(std::istream_iterator<T>(std::cin), 5, 
                filtered_values_.begin());
    }

    bool is_user_queried_;
    std::array<T, 5> filtered_values_;
};
