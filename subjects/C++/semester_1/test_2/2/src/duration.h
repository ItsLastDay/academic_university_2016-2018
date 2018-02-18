#pragma once

#include <cstdint>

namespace chrono
{

const uintmax_t ONE_SECOND = 1;

struct duration
{
    duration(intmax_t ticks, uintmax_t secs_per_tick);
    duration(intmax_t secs);
    duration();
    duration(duration const& other);

    intmax_t ticks() const;
    uintmax_t secs_per_tick() const;

    duration& operator+=(duration const& other);
    duration& operator-=(duration const& other);
    duration& operator*=(uintmax_t n);
    duration& operator/=(uintmax_t n);

private:
    intmax_t ticks_;
    uintmax_t secs_per_tick_;
};

duration seconds(intmax_t amount);
duration minutes(intmax_t amount);
duration days(intmax_t amount);

duration operator+(duration cur, duration const& other);
duration operator-(duration cur, duration const& other);
duration operator*(duration cur, uintmax_t n);
duration operator/(duration cur, uintmax_t n);
duration operator*(uintmax_t n, duration cur);
duration operator/(uintmax_t n, duration cur);

uintmax_t operator/(duration const& cur, duration const& other);

} // namespace chrono
