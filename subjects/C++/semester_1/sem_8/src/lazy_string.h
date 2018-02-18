#pragma once

#include "shared_buffer.h"

#include <cstddef>
#include <iostream>

namespace lazy
{

struct lazy_string
{
    lazy_string(const char *src);
    lazy_string(lazy_string const &src);
    lazy_string& operator=(lazy_string src);
    void set_at(size_t ix, char value);
    
    bool empty() const;
    size_t get_size() const;
    char get_at(size_t ix) const;

    struct char_assigner
    {
        char_assigner(lazy_string& str, size_t idx);
        void operator=(char c);
        operator char();

    private:
        lazy_string& str_;
        size_t idx_;
    };

    char operator[](size_t idx) const;
    char_assigner operator[](size_t idx);
    
private:
    friend void print(std::ostream& os, const lazy_string& str);
    friend lazy_string concat(const lazy_string &str1, 
            const lazy_string &str2);
    shared_buffer buf_;
};

lazy_string& operator+=(lazy_string& str, lazy_string const& other);
lazy_string operator+(lazy_string str, lazy_string const& other);
bool operator<(lazy_string const& lhs, lazy_string const& rhs);

std::ostream& operator<<(std::ostream&, const lazy_string&);
size_t find(const lazy_string& in, const char* what, size_t start_ix=0);

} // namespace lazy
