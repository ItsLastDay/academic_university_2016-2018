#pragma once
#include <string>

struct my_ostream
{
    virtual my_ostream& operator<<(int) = 0;
    virtual my_ostream& operator<<(double) = 0;
    virtual my_ostream& operator<<(std::string) = 0;

    virtual ~my_ostream() {};
};
