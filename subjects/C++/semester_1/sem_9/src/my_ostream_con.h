#pragma once

#include <string>

#include "my_ostream.h"

struct my_ostream_con
    : my_ostream
{
    my_ostream_con& operator<<(int) override;
    my_ostream_con& operator<<(double) override;
    my_ostream_con& operator<<(std::string) override;
};
