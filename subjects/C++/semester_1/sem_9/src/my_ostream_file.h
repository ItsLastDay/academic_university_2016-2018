#pragma once

#include <fstream>
#include <string>

#include "my_ostream.h"

struct my_ostream_file
    : my_ostream
{
    my_ostream_file(const char* filename);

    my_ostream_file& operator<<(int) override;
    my_ostream_file& operator<<(double) override;
    my_ostream_file& operator<<(std::string) override;

private:
    std::ofstream file_out_;
};
