#pragma once

#include "big_integer.h"

#include <cctype>

#include <ostream>
#include <istream>
#include <string>

namespace apa
{

struct rational
{
    rational(rational const&);
    explicit rational(int numerator=0, int denominator=1);
    explicit rational(std::string numerator, std::string denominator="1");

    void swap(rational &other);
    rational& operator=(rational);

    rational& operator+=(rational const& rhs);
    rational& operator*=(rational const& rhs);
    rational& operator/=(rational const& rhs);

    std::string str() const;
    explicit operator bool() const;
private:
    void normalize();
    // In order to omit getters for `numerator_` and `denominator_`, make 
    // `operator<<` and others friends.
    friend std::ostream& operator<<(std::ostream& out, rational const& number);
    friend bool operator==(rational const& lhs, rational const& rhs);
    friend bool operator>(rational const& lhs, rational const& rhs);
    big_integer numerator_, denominator_;
};


std::ostream& operator<<(std::ostream& out, rational const& number);
std::istream& operator>>(std::istream& in, rational &number);

rational operator+(rational lhs, rational const& rhs);
rational operator*(rational lhs, rational const& rhs);
rational operator/(rational lhs, rational const& rhs);

bool operator==(rational const& lhs, rational const& rhs);
bool operator!=(rational const& lhs, rational const& rhs);
bool operator>(rational const& lhs, rational const& rhs);
bool operator>=(rational const& lhs, rational const& rhs);
bool operator<(rational const& lhs, rational const& rhs);
bool operator<=(rational const& lhs, rational const& rhs);

} // namespace apa
