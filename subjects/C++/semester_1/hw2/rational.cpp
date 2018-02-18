#include "rational.h"
#include "big_integer.h"

#include <string>


namespace apa
{

rational::rational(int numerator, int denominator)
    : numerator_(numerator)
    , denominator_(denominator)
{
    normalize();
}


rational::rational(std::string numerator, std::string denominator)
    : numerator_(numerator)
    , denominator_(denominator)
{
    normalize();
}


rational::rational(rational const& other)
    : numerator_(other.numerator_)
    , denominator_(other.denominator_)
{
}


void rational::swap(rational &other)
{
    numerator_.swap(other.numerator_);
    denominator_.swap(other.denominator_);
}


rational& rational::operator=(rational other)
{
    swap(other);
    return *this;
}


rational::operator bool() const
{
    return numerator_ != big_integer(0);

}

std::ostream& operator<<(std::ostream& out, rational const& number)
{
    out << number.str();
    return out;
}


std::istream& operator>>(std::istream& in, rational &number)
{
    std::string numerator, slash, denominator;
    in >> numerator >> slash >> denominator;

    if (slash != "/" or numerator.empty() or denominator.empty())
    {
        throw wrong_format();
    }

    number = rational(numerator, denominator);
    return in;
}


void rational::normalize()
{
    auto g = gcd(numerator_, denominator_);
    if (g != big_integer(1))
    {
        numerator_ /= g;
        denominator_ /= g;
    }
}


rational& rational::operator+=(rational const& rhs)
{
    if (this == &rhs)
    {
        numerator_ += numerator_;
        normalize();
        return *this;
    }

    numerator_ *= rhs.denominator_;
    numerator_ += denominator_ * rhs.numerator_;
    denominator_ *= rhs.denominator_;
    normalize(); 
    return *this;
}


rational& rational::operator*=(rational const& rhs)
{
    numerator_ *= rhs.numerator_;
    denominator_ *= rhs.denominator_;
    normalize();
    return *this;
}


rational& rational::operator/=(rational const& rhs)
{
    if (this == &rhs)
    {
        *this = rational(1, 1);
        return *this;
    }

    numerator_ *= rhs.denominator_;
    denominator_ *= rhs.numerator_;
    normalize();
    return *this;
}


rational operator+(rational lhs, rational const& rhs)
{
    lhs += rhs;
    return lhs;
}


rational operator*(rational lhs, rational const& rhs)
{
    lhs *= rhs;
    return lhs;
}


rational operator/(rational lhs, rational const& rhs)
{
    lhs /= rhs;
    return lhs;
}


bool operator==(rational const& lhs, rational const& rhs)
{
    return lhs.numerator_ == rhs.numerator_ and
        lhs.denominator_ == rhs.denominator_;
}


bool operator!=(rational const& lhs, rational const& rhs)
{
    return not (lhs == rhs);
}


bool operator>(rational const& lhs, rational const& rhs)
{
    return lhs.numerator_ * rhs.denominator_ > 
        lhs.denominator_ * rhs.numerator_;
}


bool operator>=(rational const& lhs, rational const& rhs)
{
    return lhs > rhs or lhs == rhs;
}


bool operator<(rational const& lhs, rational const& rhs)
{
    return rhs > lhs;
}


bool operator<=(rational const& lhs, rational const& rhs)
{
    return rhs >= lhs;
}


std::string rational::str() const
{
    return numerator_.str() + " / " + denominator_.str();
}


} // namespace apa
