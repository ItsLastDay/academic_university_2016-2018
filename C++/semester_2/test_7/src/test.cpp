#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "safe_dl.h"
#include <algorithm>

using namespace safe_dl;
using std::vector;
using std::pair;
using std::string;

int identity(int x)
{
    return x;
}

float plus_one(float x)
{
    return x + 1.f;
}

#ifndef TASK3

registrator register_identity("identity", identity);
registrator register_plus_one("plus_one", plus_one);

#else

REG_FUNCTION(identity);
REG_FUNCTION(plus_one);

#endif

void check_registration()
{
    vector<pair<string, void*>> expected = 
    {
        {"identity", (void*)identity},
        {"plus_one", (void*)plus_one}
    };

    auto given = get_functions();
    sort(given.begin(), given.end());

    CHECK(expected == given);   
}

TEST_CASE("Test registration")
{
    check_registration();
}

#ifdef TASK2

TEST_CASE("Test registration preload")
{
    load_function<int(int)>("shared.so", "identity");
    check_registration();
}

TEST_CASE("Test loading")
{
    auto fake_id = load_function<int(int)>("shared.so", "identity");
    std::cout << "bla" << std::endl;
    CHECK(fake_id);
    CHECK(fake_id(10) == 0); 
}

TEST_CASE("Test incorrect loads")
{
    CHECK(!load_function<int(int)>("abacaba", "identity"));
    CHECK(!load_function<void(int)>("shared.so", "identity"));
    CHECK(!load_function<int(int)>("shared.so", "wrong_name"));
}

TEST_CASE("Test call from different libs")
{
    auto fake_id = load_function<int(int)>("shared.so", "identity");
    auto id = load_function<int(int)>("shared2.so", "identity");
    
    CHECK(id);
    CHECK(fake_id);
    CHECK(fake_id(10) == 0);
    CHECK(id(10) == 10);

    // What if something depend on order / reloading
    id = load_function<int(int)>("shared2.so", "identity");
    fake_id = load_function<int(int)>("shared.so", "identity");
    
    CHECK(id);
    CHECK(fake_id);
    CHECK(fake_id(10) == 0);
    CHECK(id(10) == 10);
}

TEST_CASE("Test many arguments")
{
    auto fun = load_function<int(int, float, double, short)>("shared.so", "many_args");
    CHECK(fun);
}

#endif
