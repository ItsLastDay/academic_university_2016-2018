#include "big_integer.h"

#include <cassert>
#include <cctype>

#include <algorithm>
#include <string>


namespace apa
{

big_integer::big_integer(uint32_t number)
{
    digits_.push_back(number);
}


big_integer::big_integer(std::string number)
{
    for (auto const& c: number)
    {
        if (not isdigit(c))
        {
            throw wrong_format();
        }
    }

    if (number == "0")
    {
        digits_.push_back(0);
    }
    else
    {
        std::reverse(number.begin(), number.end());
        auto binary_rev_string = to_binary_reversed_string(number);
        size_t sz = binary_rev_string.size();

        for (size_t i = 0; i < sz; i += 32)
        {
            uint32_t cur_digit = 0;
            for (size_t j = std::min(i + 32, sz); j > i; j--)
            {
                cur_digit = cur_digit * 2 + (binary_rev_string[j - 1] - '0');
            }

            digits_.push_back(cur_digit);
        }
    }
}


std::string big_integer::to_binary_reversed_string(std::string reversed_decimal_string)
{
    if (reversed_decimal_string.empty())
    {
        return "";
    }

    size_t sz = reversed_decimal_string.size();
    short last_digit_parity = (reversed_decimal_string[0] - '0') & 1;
    std::string last_digit(1, last_digit_parity + '0');

    // Divide by two.
    short carry = 0;
    for (int i = sz - 1; i >= 0; i--)
    {
        short digit = reversed_decimal_string[i] - '0' + 10 * carry;
        carry = digit & 1;
        digit /= 2;
        reversed_decimal_string[i] = digit + '0';
    }

    if (reversed_decimal_string.back() == '0')
    {
        reversed_decimal_string.pop_back();
    }

    return last_digit + to_binary_reversed_string(reversed_decimal_string);
}


big_integer::big_integer(big_integer const& other)
    : digits_(other.digits_)
{
}


void big_integer::swap(big_integer& other)
{
    digits_.swap(other.digits_);
}


big_integer& big_integer::operator=(big_integer other)
{
    swap(other);
    return *this;
}


void big_integer::add_at_pos(size_t pos, uint64_t val)
{
    uint64_t carry = val;

    while (carry)
    {
        while (length() <= pos)
        {
            digits_.push_back(0);
        }

        uint32_t cur_digit = get_digit(pos);
        uint64_t sum = carry % number_base_;
        carry /= number_base_;
        sum += cur_digit;
        digits_[pos] = sum % number_base_;
        carry += sum / number_base_;
        ++pos;
    }
}


big_integer& big_integer::operator+=(uint32_t val)
{
    add_at_pos(0, val);
    return *this;
}


big_integer& big_integer::operator+=(big_integer const& rhs)
{
    size_t size_right = rhs.length();

    for (size_t i = 0; i < size_right; i++)
    {
        uint32_t right_dig = rhs.get_digit(i);
        add_at_pos(i, right_dig);
    }

    return *this;
}


big_integer& big_integer::operator-=(big_integer const& rhs)
{
    assert(*this >= rhs);

    size_t sz = rhs.length();
    int64_t carry = 0;

    for (size_t i = 0; carry or i < sz; i++)
    {
        uint64_t digit_left = get_digit(i);
        uint64_t digit_right = i < sz ? rhs.get_digit(i) : 0;

        int64_t sum = carry;
        sum += digit_left - digit_right;

        carry = 0;
        while (sum < 0)
        {
            sum += number_base_;
            carry--;
        }

        digits_[i] = sum;
    }

    normalize();
    return *this;
}


void big_integer::normalize()
{
    while (length() > 0 and get_digit(length() - 1) == 0)
    {
        digits_.pop_back();
    }
}


big_integer& big_integer::operator*=(big_integer const& rhs)
{
    size_t size_right = rhs.length();
    size_t size_left = length();
    big_integer result(0);

    for (size_t i = 0; i < size_right; i++)
    {
        for (size_t j = 0; j < size_left; j++)
        {
            uint64_t digit_prod = rhs.get_digit(i);
            digit_prod *= digits_[j];
            result.add_at_pos(i + j, digit_prod);
        }
    }

    swap(result);
    return *this;
}


big_integer& big_integer::div_by_2()
{
    size_t sz = length();

    // Since we operate modulo 2**32, we can divide by 2 via bitshifting.
    for (size_t i = 0; i < sz; i++)
    {
        bool lowest_bit = get_digit(i) & 1;

        digits_[i] = digits_[i] >> 1;
        if (i > 0)
        {
            // Propagate the lowest bit to previous digit.
            digits_[i - 1] = digits_[i - 1] | ((1u << 31) * lowest_bit);
        }
    }

    normalize();
    return *this;
}


big_integer get_quotient(big_integer const& dividend, big_integer const& divisor)
{
    // Search for highest `x` such that `divisor` * `x` <= `dividend`
    // via binary search.
    // Invariant: `x` * `left` <= `dividend`
    //            `x` * `right` > `dividend`
    
    big_integer left(0);
    big_integer right(dividend + 1);

    while (left + 1 < right)
    {
        auto mid = (left + right).div_by_2();
        if (mid * divisor <= dividend)
        {
            left = mid;
        }
        else
        {
            right = mid;
        }
    }

    return left;
}


big_integer& big_integer::operator/=(big_integer const& rhs)
{
    // If we are to divide by zero, simulate a standard
    // behaviour. It can be different on different compilers,
    // so we really need to divide by zero.
    if (rhs == apa::big_integer(0))
    {
        int x = 1;
        x /= x - 1;
        // If `x` is not used, it will be optimized out.
        apa::big_integer tmp(x);
        *this += tmp;
        return *this;
    }

    auto quotient = get_quotient(*this, rhs);
    swap(quotient);
    return *this;
}


big_integer gcd(big_integer lhs, big_integer rhs)
{
    if (lhs < rhs)
    {
        std::swap(lhs, rhs);
    }

    while (rhs > big_integer(0))
    {
        auto tmp = lhs % rhs;
        lhs = rhs;
        rhs = tmp;
    }

    return lhs;
}


big_integer& big_integer::operator%=(big_integer const& rhs)
{
    auto quotient = get_quotient(*this, rhs);
    quotient *= rhs;
    *this -= quotient;
    return *this;
}


size_t big_integer::length() const
{
    return digits_.size();
}


uint32_t big_integer::get_digit(size_t idx) const
{
    return digits_[idx];
}



big_integer operator*(big_integer lhs, big_integer const& rhs)
{
    lhs *= rhs;
    return lhs;
}


big_integer operator-(big_integer lhs, big_integer const& rhs)
{
    lhs -= rhs;
    return lhs;
}


big_integer operator+(big_integer lhs, big_integer const& rhs)
{
    lhs += rhs;
    return lhs;
}


big_integer operator+(big_integer lhs, uint32_t val)
{
    lhs += val;
    return lhs;
}


big_integer operator%(big_integer lhs, big_integer const& rhs)
{
    lhs %= rhs;
    return lhs;
}


big_integer operator/(big_integer lhs, big_integer const& rhs)
{
    lhs /= rhs;
    return lhs;
}


std::string big_integer::str() const
{
    // First convert to base 2, then to base 10.
    // Works very slow.
    std::string string_repr_binary_reversed = "";
    size_t sz = length();

    for (size_t i = 0; i < sz; i++)
    {
        uint32_t digit = get_digit(i);
        std::string digit_repr_reversed = "";
        // Leading zeroes must be present for all digits except the first
        // (which is the last in `digits_` array, because number is
        // stored backwards).
        for (size_t j = 0; digit or (j < 32 and i + 1 < sz); j++)
        {
            digit_repr_reversed += digit % 2 + '0';
            digit /= 2;
        }
        string_repr_binary_reversed += digit_repr_reversed;
    }

    std::string string_repr_decimal_reversed = "0";

    size_t sz_binary = string_repr_binary_reversed.size();
    for (size_t i = sz_binary; i > 0; i--)
    {
        // Multiply by two.
        size_t cur_decimal_size = string_repr_decimal_reversed.size();
        short carry = 0;
        for (size_t j = 0; j < cur_decimal_size; j++)
        {
            short cur_digit = string_repr_decimal_reversed[j] - '0';
            cur_digit *= 2;
            cur_digit += carry;
            carry = cur_digit / 10;
            cur_digit %= 10;
            string_repr_decimal_reversed[j] = cur_digit + '0';
        }
        if (carry)
        {
            string_repr_decimal_reversed += carry + '0';
        }

        // Add zero or one.
        carry = string_repr_binary_reversed[i - 1] - '0';
        cur_decimal_size = string_repr_decimal_reversed.size();
        for (size_t j = 0; carry and j < cur_decimal_size; j++)
        {
            short cur_digit = string_repr_decimal_reversed[j] - '0' + carry;
            carry = cur_digit / 10;
            cur_digit %= 10;
            string_repr_decimal_reversed[j] = cur_digit + '0';
        }
        if (carry)
        {
            string_repr_decimal_reversed += carry + '0';
        }
    }

    std::reverse(string_repr_decimal_reversed.begin(), string_repr_decimal_reversed.end());
    return string_repr_decimal_reversed;
}


bool operator==(big_integer const& lhs, big_integer const& rhs)
{
    if (lhs.length() != rhs.length())
    {
        return false;
    }

    size_t sz = lhs.length();
    for (size_t i = 0; i < sz; i++)
    {
        if (lhs.get_digit(i) != rhs.get_digit(i))
        {
            return false;
        }
    }

    return true;
} 


bool operator!=(big_integer const& lhs, big_integer const& rhs)
{
    return not (lhs == rhs);
}


bool operator<(big_integer const& lhs, big_integer const& rhs)
{
    if (lhs.length() != rhs.length())
    {
        return lhs.length() < rhs.length();
    }

    size_t sz = lhs.length();
    for (size_t i = sz; i > 0; i--)
    {
        uint32_t digit_left = lhs.get_digit(i - 1);
        uint32_t digit_right = rhs.get_digit(i - 1);
        if (digit_left != digit_right)
        {
            return digit_left < digit_right;
        }
    }

    return false;
}


bool operator<=(big_integer const& lhs, big_integer const& rhs)
{
    return lhs == rhs or lhs < rhs;
}


bool operator>(big_integer const& lhs, big_integer const& rhs)
{
    return rhs < lhs;
}


bool operator>=(big_integer const& lhs, big_integer const& rhs)
{
    return lhs == rhs or lhs > rhs;
}


big_integer::big_integer(big_integer &&other)
{
    std::swap(digits_, other.digits_);
}


} // namespace apa
