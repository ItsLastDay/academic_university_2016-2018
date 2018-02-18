#pragma once

#include "shared_buffer.h"

#include <cstddef>
#include <iostream>

struct lazy_string
{
    lazy_string(const char *src);
    lazy_string(lazy_string const &src);
    lazy_string& operator=(lazy_string src);
    void set_at(size_t ix, char value);
    
    bool empty() const;
    size_t get_size() const;
    char get_at(size_t ix) const;
    
private:
    friend void print(std::ostream& os, const lazy_string& str);
    friend lazy_string concat(const lazy_string &str1, 
            const lazy_string &str2);
    shared_buffer buf_;
};
