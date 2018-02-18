
#include <string>
#include <vector>

#include "my_ostream.h"
#include "my_ostream_combo.h"

my_ostream_combo::my_ostream_combo(std::vector<my_ostream*> stream_ptrs)
    : stream_ptrs_(stream_ptrs)
{
}

my_ostream_combo& my_ostream_combo::operator<<(int val)
{
    for (auto stream_ptr: stream_ptrs_)
    {
        (*stream_ptr) << val;
    }
    return *this;
}

my_ostream_combo& my_ostream_combo::operator<<(double val)
{
    for (auto stream_ptr: stream_ptrs_)
    {
        (*stream_ptr) << val;
    }
    return *this;
}

my_ostream_combo& my_ostream_combo::operator<<(std::string val)
{
    for (auto stream_ptr: stream_ptrs_)
    {
        (*stream_ptr) << val;
    }
    return *this;
}
