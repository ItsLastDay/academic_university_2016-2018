#include "duration.h"

namespace
{
    uintmax_t SECS_PER_MIN = 60;
    uintmax_t SECS_PER_DAY = SECS_PER_MIN * 24 * 60;
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


} // namespace chrono
