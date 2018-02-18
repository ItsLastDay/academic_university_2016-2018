#pragma once

#include <cstddef>
#include <cstring>

#include <utility>


namespace containers
{

template<class Value>
struct deque
{
    typedef Value value_type;
    typedef value_type& reference;
    typedef value_type const& const_reference;
    typedef size_t size_type;

    deque()
        : buffer_(nullptr)
        , capacity_(0)
        , cur_size_(0)
        , idx_front_(0)
        , idx_back_(0)
    {
    }

    deque(deque const& other)
        : buffer_(new value_type[other.capacity_])
        , capacity_(other.capacity_)
        , cur_size_(other.cur_size_)
        , idx_front_(other.idx_front_)
        , idx_back_(other.idx_back_)
    {
        memcpy(buffer_, other.buffer_, sizeof(value_type) * capacity_); 
    }

    deque& operator=(deque other)
    {
        swap(other);
        return *this;
    }

    ~deque()
    {
        delete[] buffer_;
    }

    void swap(deque& other)
    {
        using std::swap;
        swap(buffer_, other.buffer_);
        swap(capacity_, other.capacity_);
        swap(cur_size_, other.cur_size_);
        swap(idx_front_, other.idx_front_);
        swap(idx_back_, other.idx_back_);
    }


    size_type size() const
    {
        return cur_size_;
    }

    reference operator[](size_type idx) const
    {
        size_type begin = idx_front_;
        begin = (begin + idx) % capacity_;
        return buffer_[begin];
    }

    reference front() 
    {
        return buffer_[idx_front_];
    }

    const_reference front() const
    {
        return buffer_[idx_front_];
    }

    reference back() 
    {
        return buffer_[idx_before_back()];
    }

    const_reference back() const
    {
        return buffer_[idx_before_back()];
    }

    void pop_front()
    {
        idx_front_ = idx_after_front();
        dec_size();
    }

    void pop_back()
    {
        idx_back_ = idx_before_back();
        dec_size();
    }

    void push_front(const_reference val)
    {
        inc_size();
        idx_front_ = idx_before_front();
        buffer_[idx_front_] = val;
    }

    void push_back(const_reference val)
    {
        inc_size();
        buffer_[idx_back_] = val;
        idx_back_ = (idx_back_ + 1) % capacity_;
    }

    void resize(size_type new_size, const_reference value=value_type())
    {
        while (new_size < cur_size_)
        {
            pop_back();
        }
        while (new_size > cur_size_)
        {
            push_back(value);
        }
    }

    size_type capacity() const
    {
        return capacity_;
    }

    void reserve(size_type new_capacity)
    {
        if (capacity_ >= new_capacity)
        {
            return;
        }

        value_type* new_buffer = new value_type[new_capacity];
        memcpy(new_buffer, buffer_, sizeof(value_type) * capacity_);
        for (size_type i = 0; i < capacity_; 
                ++i, idx_front_ = (idx_front_ + 1) % capacity_)
        {
            new_buffer[i] = buffer_[idx_front_];
        }
        delete[] buffer_;
        buffer_ = new_buffer;
        capacity_ = new_capacity;
        idx_front_ = 0;
        idx_back_ = cur_size_;
    }


private:
    value_type* buffer_;
    size_type capacity_;
    size_type cur_size_;
    // front - first element, back - after last element.
    size_type idx_front_, idx_back_; 

    void dec_size()
    {
        --cur_size_;
    }

    void inc_size()
    {
        ++cur_size_;
        if (cur_size_ > capacity_)
        {
            extend();
        }
    }

    size_type idx_before_front() const
    {
        if (idx_front_ == 0)
        {
            return capacity_ - 1;
        }
        return idx_front_ - 1;
    }

    size_type idx_after_front() const
    {
        return (idx_front_ + 1) % capacity_;
    }

    size_type idx_before_back() const
    {
        if (idx_back_ == 0)
        {
            return capacity_ - 1;
        }
        return idx_back_ - 1;
    }

    void extend()
    {
        size_type new_capacity = capacity_ ? capacity_ * 2 : 1;
        value_type* new_buffer = new value_type[new_capacity];
        size_type copied_idx = 0;
        for (size_type i = 0; i < capacity_; 
                ++i, idx_front_ = (idx_front_ + 1) % capacity_, ++copied_idx)
        {
            new_buffer[copied_idx] = buffer_[idx_front_];
        }

        capacity_ = new_capacity;
        delete[] buffer_;
        buffer_ = new_buffer;
        idx_front_ = 0;
        idx_back_ = copied_idx;
    }
};

template<class T>
bool operator<(deque<T> const& me, deque<T> const& other)
{
    typename deque<T>::size_type min_sz = me.size();
    if (min_sz > other.size())
    {
        min_sz = other.size();
    }
    for (typename deque<T>::size_type i = 0; i < min_sz; ++i)
    {
        if (me[i] != other[i])
        {
            return me[i] < other[i];
        }
    }
    return me.size() < other.size();
}

template<class T>
bool operator==(deque<T> const& me, deque<T> const& other)
{
    if (me.size() != other.size())
    {
        return false;
    }

    for (typename deque<T>::size_type i = 0; i < me.size(); ++i)
    {
        if (me[i] != other[i])
        {
            return false;
        }
    }

    return true;
}

template<class T>
bool operator!=(deque<T> const& me, deque<T> const& other)
{
    return not (other == me);
}

template<class T>
bool operator>(deque<T> const& me, deque<T> const& other)
{
    return other < me;
}

template<class T>
bool operator>=(deque<T> const& me, deque<T> const& other)
{
    return other == me or other < me;
}

template<class T>
bool operator<=(deque<T> const& me, deque<T> const& other)
{
    return other == me or me < other;
}

} // namespace containers
