#pragma once

#include <stddef.h>

#include <forward_list>

template<class T>
struct fifo
{
    fifo()
        : cur_size_(0)
    {
    }

    fifo(fifo const&) = default;

    fifo& operator=(fifo other)
    {
        swap(other);
        return *this;
    }

    void swap(fifo & other)
    {
        std::swap(stack_insert_, other.stack_insert_);
        std::swap(stack_pop_, other.stack_pop_);
        std::swap(cur_size_, other.cur_size_);
    }

    void push(T const& val)
    {
        ++cur_size_;
        stack_insert_.push_front(val);
    }
    
    void pop()
    {
        --cur_size_;
        prepare_for_pop();
        stack_pop_.pop_front();
    }


    T& front()
    {
        prepare_for_pop();
        return stack_pop_.front();
    }


    const T& front() const
    {
        prepare_for_pop();
        return stack_pop_.front();
    }


    typename std::forward_list<T>::size_type
    size() const
    {
        return cur_size_;
    }

    ~fifo()
    {
        while (size() > 0)
        {
            pop();
        }
    }
private:
    void prepare_for_pop()
    {
        if (stack_pop_.empty())
        {
            while (not stack_insert_.empty())
            {
                stack_pop_.push_front(stack_insert_.front());
                stack_insert_.pop_front();
            }
        }
    }

    typename std::forward_list<T>::size_type cur_size_;
    std::forward_list<T> stack_insert_, stack_pop_;
};
