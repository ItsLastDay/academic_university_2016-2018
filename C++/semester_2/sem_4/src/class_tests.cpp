#include <iostream>
#include <utility>
#include <string>
#include <functional>
#include <cassert>

#include "event.h"

static void test_task1()
{
    size_t eh_calls_cnt = 0;
    utils::event<int(const std::string&, const std::string&)> event;

    auto event_handler_printer = [&eh_calls_cnt](const std::string &str1, const std::string &str2) {
        std::cout << "Event handler " << str1 << " | " << str2 << std::endl;
        ++eh_calls_cnt;
        return eh_calls_cnt;
    };

    event.add(event_handler_printer);
    event.add(event_handler_printer);
    event.add([](const std::string &s1, const std::string &s2)
            {
            std::cout << "bla bla " << s1 << s2;
            return 0;
            });
    event.fire("fire the", "1st time");
    assert(eh_calls_cnt == 2);
}


static void test_task2()
{
    std::cout << std::endl;
    size_t eh_calls_cnt = 0;
    utils::event<int(const std::string&, const std::string&)> event;

    auto event_handler_printer = [&eh_calls_cnt](const std::string &str1, const std::string &str2) {
        std::cout << "Event handler " << str1 << " | " << str2 << std::endl;
        ++eh_calls_cnt;
        return eh_calls_cnt;
    };

    auto eh_del1 = event.add(event_handler_printer);
    auto eh_del2 = event.add(event_handler_printer);
    auto eh_del3 = event.add(event_handler_printer);

    auto del2_copy = eh_del2;

    assert(eh_calls_cnt == 0);
    event.fire("fire the", "1st time");
    assert(eh_calls_cnt == 3);
    eh_del2();
    event.fire("fire the", "2nd time");
    assert(eh_calls_cnt == 5);

    (void)eh_del1;
    (void)eh_del2;
    (void)eh_del3;
}


static void test_task3()
{
    using namespace std::placeholders;  // for _1, _2, _3...
    utils::event<void(const std::string&, const std::string&)> event;

    auto event_handler_printer = [](const std::string &str1,
            const std::string &str2,
            const std::string &str3) {
        std::cout << "Event handler " << str1 << " | " << str2 << " | " << str3 << std::endl;
    };

    std::string binded_str = "I am binded!";
    event.add(std::bind(event_handler_printer, binded_str, _1, _2));
    event.add(std::bind(event_handler_printer, _1, binded_str, _2));
    event.add(std::bind(event_handler_printer, _1, _2, binded_str));
    event.fire("I am arg1", "I am arg2");
}


static void test_task4()
{
    utils::event<void(const std::string&)> event1;
    utils::event<void(const std::string&, int)> event2;
    utils::event<void(const std::string&, int, int)> event3;
    utils::event<void(const std::string&, int, int, int)> event4;
    utils::event<void(const std::string&, int, int, int, std::string&)> event5;
    event3.add([](const std::string &s1, int x, int y)
            {
            std::cout << s1 << " " << x << " " << y << std::endl;
            });
    event3.fire("x", 1, 2);
    (void)event1;
    (void)event2;
    (void)event3;
    (void)event4;
    (void)event5;
}


int main()
{
    test_task1();
    test_task2();
    test_task3();
    test_task4();
    return 0;
}
