#include <cassert>
#include <sstream>
#include <cmath>

#include "duration.h"

#define TEST_TASK1

using namespace chrono;

void check_duration_components_eq(const duration &d1, const duration &d2)
{
    assert(d1.ticks() == d2.ticks());
    assert(d1.secs_per_tick() == d2.secs_per_tick());
}

static void test_constructors()
{
#ifdef TEST_TASK1
    duration d1;
    assert(d1.ticks() == 0);
    assert(d1.secs_per_tick() == 1);

    duration d2(10);
    assert(d2.ticks() == 10);
    assert(d2.secs_per_tick() == 1);

    duration d3(101, 99);
    assert(d3.ticks() == 101);
    assert(d3.secs_per_tick() == 99);

    const duration &d5 = seconds(100);
    assert(d5.ticks() == 100);
    assert(d5.secs_per_tick() == 1);

    const duration &d6 = minutes(777);
    assert(d6.ticks() == 777);
    assert(d6.secs_per_tick() == 60);

    const duration &d4 = days(1001);
    assert(d4.ticks() == 1001);
    assert(d4.secs_per_tick() == 60 * 60 * 24);
#endif // TEST_TASK1
}

static void test_copy_and_assignment()
{
#ifdef TEST_TASK1
    duration d1(100, 200);
    duration d2(d1);
    check_duration_components_eq(d1, d2);
    d2 = duration(500, 1000);
    assert(d2.ticks() == 500);
    assert(d2.secs_per_tick() == 1000);
#endif // TEST_TASK1
}

static void test_constants()
{
#ifdef TEST_TASK1
    check_duration_components_eq(chrono::ONE_SECOND, duration(1, 1));
#endif // TEST_TASK1
}

static void test_arithmetic_operators()
{
#ifdef TEST_TASK2
    duration d1(1000, 1);
    duration d2(d1);
    duration d3(500, 2);
    duration d4;

    d2 += d3;
    assert(d2.ticks() == 2000);
    assert(d2.secs_per_tick() == 1);
    d4 = d1 + d3;
    check_duration_components_eq(d2, d4);

    d2 = d1;
    d2 -= d3;
    assert(d2.ticks() == 0);
    assert(d2.secs_per_tick() == 1);
    d4 = d1 - d3;
    check_duration_components_eq(d2, d4);

    d2 = d1;
    d2 *= 10;
    assert(d2.ticks() == 10000);
    assert(d2.secs_per_tick() == 1);
    d4 = d1 * 10;
    check_duration_components_eq(d2, d4);
    d4 = 10 * d1;
    check_duration_components_eq(d2, d4);

    d2 = d1;
    d2 /= 10;
    assert(d2.ticks() == 100);
    assert(d2.secs_per_tick() == 1);
    d4 = d1 / 10;
    check_duration_components_eq(d2, d4);

    duration d5(1000, 1);
    duration d6(1, 1000);
    assert((d5 / d6) == 1);

    check_duration_components_eq(minutes(1) + seconds(30), seconds(90));
#endif // TEST_TASK2
}

static void test_relational_operators()
{
#ifdef TEST_TASK3
    duration d1(1, 2);
    duration d2(2, 1);
    duration d3(1, 3);
    duration d4(4, 2);
    assert(d1 == d1);
    assert(d1 == d2);
    assert(d1 >= d2);
    assert(d1 <= d2);
    assert(d1 >= d2);
    assert(d3 > d1);
    assert(d3 >= d1);
    assert(d3 < d4);
    assert(d3 <= d4);
#endif // TEST_TASK3
}

static void test_io_operators()
{
#ifdef TEST_TASK3
    std::stringstream ss;
    duration d1(1777, 999);
    duration d2;
    ss << d1 << " ";
    ss << d1 << " ";
    ss << d1 << " ";
    ss >> d2;
    ss >> d2;
    assert(d1 == d2);
#endif // TEST_TASK3
}

int main()
{
    test_constructors();
    test_copy_and_assignment();
    test_constants();
    test_arithmetic_operators();
    test_relational_operators();
    test_io_operators();
    return 0;
}
