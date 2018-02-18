#include <string>

#include "my_ostream_file.h"

my_ostream_file::my_ostream_file(const char* filename)
    : file_out_(filename)
{
}

my_ostream_file& my_ostream_file::operator<<(int val)
{
    file_out_ << val;
    return *this;
}

my_ostream_file& my_ostream_file::operator<<(double val)
{
    file_out_ << val;
    return *this;
}

my_ostream_file& my_ostream_file::operator<<(std::string val)
{
    file_out_ << val;
    return *this;
}
