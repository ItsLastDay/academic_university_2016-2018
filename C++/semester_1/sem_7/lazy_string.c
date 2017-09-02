#include "lazy_string.h"

#include <cstring>
#include <cassert>


lazy_string::lazy_string(const char* src)
    : buf_(strlen(src) + 1)
{
    char* buf = buf_.get_data();
    memcpy(buf, src, buf_.get_size());
}

lazy_string::lazy_string(lazy_string const& src)
    : buf_(src.buf_)
{
}

bool lazy_string::empty() const
{
    return buf_.get_size() == 1;
}

size_t lazy_string::get_size() const
{
    return buf_.get_size() - 1;
}

char lazy_string::get_at(size_t ix) const
{
    return buf_.get_data()[ix];
}

void print(std::ostream& os, lazy_string const& str)
{
    for (size_t i = 0; i < str.get_size(); i++)
    {
        os << str.get_at(i);
    }
}

lazy_string concat(lazy_string const& str1,
        lazy_string const& str2)
{
    size_t size_str1 = str1.buf_.get_size() - 1;
    size_t size_str2 = str2.buf_.get_size() - 1;
    shared_buffer concat_buf(size_str1 + size_str2 + 1);

    memcpy(concat_buf.get_data(), str1.buf_.get_data(), size_str1);
    memcpy(concat_buf.get_data(), str2.buf_.get_data(), size_str2 + 1);

    return lazy_string(concat_buf.get_data());
}


lazy_string& lazy_string::operator=(lazy_string src)
{
    swap(buf_, src.buf_);
    return *this;
}

void lazy_string::set_at(size_t idx, char value)
{
    assert(idx <= get_size());

    shared_buffer modified_buf(buf_.get_size()); 
    memcpy(modified_buf.get_data(), buf_.get_data(), buf_.get_size());

    buf_.get_data()[idx] = value;
    swap(buf_, modified_buf);
}



