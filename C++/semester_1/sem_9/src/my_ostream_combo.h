#pragma once

#include <string>
#include <vector>

#include "my_ostream.h"

struct my_ostream_combo
    : my_ostream
{
    my_ostream_combo(std::vector<my_ostream*> stream_ptrs);

    my_ostream_combo& operator<<(int) override;
    my_ostream_combo& operator<<(double) override;
    my_ostream_combo& operator<<(std::string) override;

private:
    std::vector<my_ostream*> stream_ptrs_;
};
