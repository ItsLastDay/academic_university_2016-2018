#include <iostream>

#include "my_ostream_con.h"


my_ostream_con& my_ostream_con::operator<<(int val)
{
    std::cout << val; 
    return *this;
}


my_ostream_con& my_ostream_con::operator<<(double val)
{
    std::cout << val;
    return *this;
}

my_ostream_con& my_ostream_con::operator<<(std::string val)
{
    std::cout << val;
    return *this;
}

