#include "duration.h"
#include <cctype>

namespace
{
    uintmax_t SECS_PER_MIN = 60;
    uintmax_t SECS_PER_DAY = SECS_PER_MIN * 24 * 60;

    uintmax_t gcd(uintmax_t x, uintmax_t y)
    {
        while(y)
        {
            uintmax_t tmp = x % y;
            x = y;
            y = tmp;
        }
        return x;
    }
} // anon namespacee

namespace chrono
{

duration::duration()
    : ticks_(0)
    , secs_per_tick_(ONE_SECOND)
{
}

duration::duration(intmax_t secs)
    : ticks_(secs)
    , secs_per_tick_(ONE_SECOND)
{
}

duration::duration(intmax_t ticks, uintmax_t secs_per_tick)
    : ticks_(ticks)
    , secs_per_tick_(secs_per_tick)
{
}

duration::duration(duration const& other)
    : ticks_(other.ticks_)
    , secs_per_tick_(other.secs_per_tick_)
{
}

intmax_t duration::ticks() const
{
    return ticks_;
}

uintmax_t duration::secs_per_tick() const
{
    return secs_per_tick_;
}


duration seconds(intmax_t amount)
{
    return duration(amount);
}

duration minutes(intmax_t amount)
{
    return duration(amount, SECS_PER_MIN);
}

duration days(intmax_t amount)
{
    return duration(amount, ONE_SECOND * SECS_PER_DAY);
}

duration& duration::operator+=(duration const& other)
{
    uintmax_t new_secs_per_tick = gcd(secs_per_tick(),
                                      other.secs_per_tick());

    intmax_t new_ticks = ticks() * (secs_per_tick() / new_secs_per_tick);
    new_ticks += other.ticks() * (other.secs_per_tick() / new_secs_per_tick);
    
    ticks_ = new_ticks;
    secs_per_tick_ = new_secs_per_tick;

    return *this;
}

duration& duration::operator-=(duration const& other)
{
    uintmax_t new_secs_per_tick = gcd(secs_per_tick(),
                                      other.secs_per_tick());

    intmax_t new_ticks = ticks() * (secs_per_tick() / new_secs_per_tick);
    new_ticks -= other.ticks() * (other.secs_per_tick() / new_secs_per_tick);
    
    ticks_ = new_ticks;
    secs_per_tick_ = new_secs_per_tick;

    return *this;
}

duration& duration::operator*=(uintmax_t n)
{
    ticks_ *= n;
    return *this;
}

duration& duration::operator/=(uintmax_t n)
{
    ticks_ /= n;
    return *this;
}

duration operator+(duration cur, duration const& other)
{
    cur += other;
    return cur;
}

duration operator-(duration cur, duration const& other)
{
    cur -= other;
    return cur;
}

duration operator*(duration cur, uintmax_t n)
{
    cur *= n;
    return cur;
}

duration operator/(duration cur, uintmax_t n)
{
    cur /= n;
    return cur;
}

duration operator*(uintmax_t n, duration cur)
{
    return cur * n;
}

duration operator/(uintmax_t n, duration cur)
{
    return cur / n;
}

uintmax_t operator/(duration const& cur, duration const& other)
{
    return static_cast<uintmax_t>(to_seconds(cur) / to_seconds(other));
}

intmax_t to_seconds(duration const& cur)
{
    return cur.ticks() * cur.secs_per_tick();
}

bool operator<(duration const& cur, duration const& other)
{
    return to_seconds(cur) < to_seconds(other);
}

bool operator<=(duration const& cur, duration const& other)
{
    return to_seconds(cur) <= to_seconds(other);
}

bool operator==(duration const& cur, duration const& other)
{
    return to_seconds(cur) == to_seconds(other);
}

bool operator>(duration const& cur, duration const& other)
{
    return other < cur;
}

bool operator>=(duration const& cur, duration const& other)
{
    return other <= cur;
}

std::ostream& operator<<(std::ostream& out, duration const& cur)
{
    out << "Duration(" << cur.ticks() << "  " << cur.secs_per_tick()
        << ")";
    return out;
}

std::istream& operator>>(std::istream& in, duration& obj)
{
    char cur_char;

    // Skip whitespaces.
    do 
    {
        in >> cur_char;
    } 
    while (isspace(cur_char));

    // Skip "Duration(" prefix;
    do
    {
        in >> cur_char;
    }
    while (cur_char != '(');

    in >> obj.ticks_ >> obj.secs_per_tick_;

    // Skip ")" suffix.
    in >> cur_char;

    return in;
}

} // namespace chrono
