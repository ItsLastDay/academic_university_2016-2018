#include "shared_buffer.h"

shared_buffer::shared_buffer(size_t size)
    : string_buf_(new char [size])
    , counter_(new int(1))
    , size_(size)
{
}

size_t shared_buffer::get_size() const
{
    return size_;
}

const char* shared_buffer::get_data() const
{
    return string_buf_;
}

char* shared_buffer::get_data()
{
    return string_buf_;
}

shared_buffer::shared_buffer(const shared_buffer& src)
{
    string_buf_ = src.string_buf_;
    counter_ = src.counter_;    
    size_ = src.size_;
    (*counter_)++;
}

shared_buffer& shared_buffer::operator=(shared_buffer src)
{
    swap(*this, src);
    return *this;
}

shared_buffer::~shared_buffer()
{
    (*counter_)--;
    if (*counter_ == 0)
    {
        delete counter_;
        delete[] string_buf_;
    }
}

void swap(shared_buffer& x, shared_buffer& y)
{
    std::swap(x.string_buf_, y.string_buf_);
    std::swap(x.counter_, y.counter_);
    std::swap(x.size_, y.size_);
}
