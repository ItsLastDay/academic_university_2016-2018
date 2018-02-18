#include "lazy_string.h"

#include <cstring>
#include <cassert>

#include <string>

namespace lazy
{

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

std::ostream& operator<<(std::ostream& os, lazy_string const& str)
{
    print(os, str);
    return os;
}

lazy_string concat(lazy_string const& str1,
        lazy_string const& str2)
{
    size_t size_str1 = str1.buf_.get_size() - 1;
    size_t size_str2 = str2.buf_.get_size() - 1;
    shared_buffer concat_buf(size_str1 + size_str2 + 1);

    memcpy(concat_buf.get_data(), str1.buf_.get_data(), size_str1);
    memcpy(concat_buf.get_data() + size_str1, 
            str2.buf_.get_data(), size_str2 + 1);

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

size_t find(const lazy_string& in, const char* what, size_t start_ix)
{
    size_t sz_this = in.get_size();
    size_t size_other = strlen(what);

    for (size_t idx = start_ix; idx + size_other <= sz_this; idx++)
    {
        bool has_matched = true;
        for (size_t i = 0; i < size_other && i + idx < sz_this; i++)
        {
            if (in.get_at(i + idx) != what[i])
            {
                has_matched = false;
                break;
            }
        }

        if (has_matched) 
        {
            return idx;
        }
    }

    return std::string::npos;
}

lazy_string& operator+=(lazy_string& str, lazy_string const& other)
{
    str = concat(str, other);
    return str;
}

lazy_string operator+(lazy_string str, lazy_string const& other)
{
    str += other;
    return str;
}


bool operator<(lazy_string const& lhs, lazy_string const& rhs)
{
    size_t lhs_size = lhs.get_size();
    size_t rhs_size = rhs.get_size();
    size_t trunc_size = lhs_size < rhs_size ? lhs_size : rhs_size;

    for (size_t i = 0; i < trunc_size; i++)
    {
        if (lhs.get_at(i) != rhs.get_at(i))
        {
            return lhs.get_at(i) < rhs.get_at(i);
        }
    }

    return lhs_size < rhs_size;
}

lazy_string::char_assigner::operator char()
{
    // Needed for "s[5] = s[6]";
    return str_[idx_];
}

char lazy_string::operator[](size_t idx) const 
{
    return get_at(idx);
}

lazy_string::char_assigner::char_assigner(lazy_string& str, size_t idx)
    : str_(str)
    , idx_(idx)
{
}

void lazy_string::char_assigner::operator=(char c)
{
    str_.set_at(idx_, c);
}

lazy_string::char_assigner lazy_string::operator[](size_t idx)
{
    return lazy_string::char_assigner(*this, idx);
}

} // namespace lazy



