#include "my_vector_int.h"

#include <cstring>
#include <cassert>

#include <algorithm>

my_vector_int::my_vector_int()
    : buffer_(new uint32_t[initial_capacity_])
    , counter_(new size_t(1))
    , size_(0)
    , capacity_(initial_capacity_)
{
}


my_vector_int::my_vector_int(my_vector_int const& other)
    : buffer_(other.buffer_)
    , counter_(other.counter_)
    , size_(other.size_)
    , capacity_(other.capacity_)
{
    ++(*counter_);
}


void my_vector_int::detach_buffer()
{
    // When we are the only one who use buffer, no need to make a copy.
    // Otherwise, detach this vector from others.
    if (*counter_ == 1)
    {
        return;
    }

    size_t* new_counter = new size_t(1);
    uint32_t* new_buffer = new uint32_t[capacity_];
    memcpy(new_buffer, buffer_, sizeof(uint32_t) * capacity_);

    --(*counter_);
    counter_ = new_counter;
    buffer_ = new_buffer;
}


void my_vector_int::push_back(uint32_t val)
{
    detach_buffer();

    if (size_ == capacity_)
    {
        extend_buffer();
    }

    buffer_[size_] = val;
    ++size_;
}


void my_vector_int::extend_buffer()
{
    size_t new_capacity = 2 * capacity_;
    uint32_t *new_buffer = new uint32_t[new_capacity];
    memcpy(new_buffer, buffer_, sizeof(uint32_t) * capacity_);
    delete[] buffer_;

    capacity_ = new_capacity;
    buffer_ = new_buffer;
}


my_vector_int::~my_vector_int()
{
    --(*counter_);
    if (*counter_ == 0)
    {
        delete[] buffer_;
        delete counter_;
    }
}


uint32_t my_vector_int::operator[](size_t idx) const
{
    assert(idx < size_);
    return buffer_[idx];
}


my_vector_int::vector_assignment_proxy::vector_assignment_proxy(size_t idx, my_vector_int& vec)
    : idx_(idx)
    , vec_(vec)
{
}


my_vector_int::vector_assignment_proxy::operator uint32_t() const
{
    return vec_.buffer_[idx_];
}


my_vector_int::vector_assignment_proxy& 
my_vector_int::vector_assignment_proxy::operator=(uint32_t val)
{
    vec_.detach_buffer();
    vec_.buffer_[idx_] = val;
    return *this;
}


my_vector_int::vector_assignment_proxy my_vector_int::operator[](size_t idx)
{
    assert(idx < size_);
    return vector_assignment_proxy(idx, *this);
}


my_vector_int& my_vector_int::operator=(my_vector_int other)
{
    swap(other);
    return *this;
}


void my_vector_int::swap(my_vector_int& other)
{
    std::swap(buffer_, other.buffer_);
    std::swap(counter_, other.counter_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
}


size_t my_vector_int::size() const
{
    return size_;
}


void my_vector_int::pop_back()
{
    // No need to detach here, because we are not making changes
    // to shared state.
    assert(size_ > 0);
    size_--;
}
