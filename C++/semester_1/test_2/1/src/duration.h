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

private:
    intmax_t ticks_;
    uintmax_t secs_per_tick_;
};

duration seconds(intmax_t amount);
duration minutes(intmax_t amount);
duration days(intmax_t amount);

} // namespace chrono
