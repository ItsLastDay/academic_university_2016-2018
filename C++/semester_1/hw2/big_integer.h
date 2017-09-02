#pragma once

#include "my_vector_int.h"

#include <string>

namespace apa
{

struct wrong_format
    : std::exception
{
};

struct big_integer
{
    big_integer& operator=(big_integer);
    big_integer(big_integer const&);
    big_integer(big_integer &&);
    explicit big_integer(uint32_t number);
    explicit big_integer(std::string number);

    big_integer& operator+=(big_integer const&);
    big_integer& operator+=(uint32_t);
    // Can only subtract numbers less-or-equal than our number.
    big_integer& operator-=(big_integer const&);
    big_integer& operator*=(big_integer const&);
    big_integer& operator/=(big_integer const&);
    big_integer& operator%=(big_integer const&);

    void swap(big_integer& other);
    big_integer& div_by_2(); 

    uint32_t get_digit(size_t idx) const;
    size_t length() const;

    std::string str() const;
private:
    void add_at_pos(size_t pos, uint64_t val);
    void normalize();

    std::string to_binary_reversed_string(std::string reversed_decimal_string);

    static const long long number_base_ = 1ll << 32;
    // `digits_[0]` is the least significant digit.
    my_vector_int digits_;
};

big_integer operator*(big_integer lhs, big_integer const& rhs);
big_integer operator+(big_integer lhs, big_integer const& rhs);
big_integer operator+(big_integer lhs, uint32_t val);
big_integer operator/(big_integer lhs, big_integer const& rhs);
big_integer operator-(big_integer lhs, big_integer const& rhs);
big_integer operator%(big_integer lhs, big_integer const& rhs);

big_integer gcd(big_integer lhs, big_integer rhs);
big_integer get_quotient(big_integer const& dividend, big_integer const& divisor);

bool operator==(big_integer const& lhs, big_integer const& rhs);
bool operator!=(big_integer const& lhs, big_integer const& rhs);
bool operator<(big_integer const& lhs, big_integer const& rhs);
bool operator<=(big_integer const& lhs, big_integer const& rhs);
bool operator>(big_integer const& lhs, big_integer const& rhs);
bool operator>=(big_integer const& lhs, big_integer const& rhs);

} // namespace apa
