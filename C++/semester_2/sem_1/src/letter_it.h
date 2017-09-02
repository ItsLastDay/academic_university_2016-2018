#pragma once

#include <iterator>

#include <cctype>


template<class fwd_it>
struct letter_iterator
{
    using it_trait = std::iterator_traits<fwd_it>;

    using value_type = char;
    using difference_type = typename it_trait::difference_type;
    using reference = char&;
    using pointer = char*;
    using iterator_category = std::forward_iterator_tag;


    letter_iterator(fwd_it begin, fwd_it end)
        : cur_begin_(begin)
        , end_(end)
    {
    }

    letter_iterator& operator++()
    {
        while (cur_begin_ != end_)
        {
            ++cur_begin_;
            if (cur_begin_ != end_ and isalpha(*cur_begin_))
                break;
        }
        return *this;
    }

    letter_iterator operator++(int)
    {
        auto it = *this;
        ++(*this);
        return it;
    }

    bool operator==(const letter_it

    reference operator*() const
    {
        return *cur_begin_;
    }
private:
    fwd_it cur_begin_;
    fwd_it end_;
};
