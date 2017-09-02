#include "standard_funcs.h"
#include "priorty_queue.h"
#include "fifo.h"

#include <iostream>

using std::cout;
using std::endl;


namespace 
{

void test_fill()
{
    cout << endl << "TEST FILL" << endl;
    std::string strings[10];
    int ints[15];
    my_stl_funcs::fill<std::string>(strings, strings + 10, "hello");
    for (int i = 0; i < 10; i++)
    {
        cout << strings[i] << " ";
    }
    cout << endl;

    my_stl_funcs::fill(ints + 5, ints + 15, 1111);


    for (int i = 0; i < 15; i++)
    {
        cout << ints[i] << " ";
    }
    cout << endl;
}

struct simple_functor_range 
{
    simple_functor_range(int start=0)
        : cur_number_(start)
    {
    }

    int operator() ()
    {
        return cur_number_++;
    }

private:
    int cur_number_;
};

void test_generate()
{
    cout << endl << "TEST GENERATE" << endl;
    int ints[15];
    auto func_returning_const = []() { return 5; };

    my_stl_funcs::generate(ints, ints + 7, func_returning_const);
    for (int i = 0; i < 15; i++)
    {
        cout << ints[i] << " ";
    }
    cout << endl;


    my_stl_funcs::generate(ints + 3, ints + 10, simple_functor_range());
    for (int i = 0; i < 15; i++)
    {
        cout << ints[i] << " ";
    }
    cout << endl;


    my_stl_funcs::generate(ints + 8, ints + 15, simple_functor_range(10));
    for (int i = 0; i < 15; i++)
    {
        cout << ints[i] << " ";
    }
    cout << endl;

    std::string strings[10];
    auto func_returning_const_str = []() { return "bla"; };
    my_stl_funcs::generate(strings, strings + 6, func_returning_const_str);
    for (int i = 0; i < 10; i++)
    {
        cout << strings[i] << " ";
    }
    cout << endl;
}

void test_copy()
{
    cout << endl << "TEST COPY" << endl;
    int ints[15];
    int ints_2[15];
    std::string strings[10];
    std::string strings_2[10];

    my_stl_funcs::fill(ints, ints + 5, 4);
    my_stl_funcs::copy(ints, ints + 5, ints_2 + 10);
    for (int i = 0; i < 15; i++)
    {
        cout << ints_2[i] << " ";
    }
    cout << endl;


    my_stl_funcs::fill(strings, strings + 10, std::string("qwer"));
    my_stl_funcs::copy(strings, strings + 5, strings_2 + 3);
    for (int i = 0; i < 10; i++)
    {
        cout << '"' << strings_2[i] << "\" ";
    }
    cout << endl;
}

struct simple_functor_sum_scan
{
    simple_functor_sum_scan()
        : sum_(0)
    {
    }

    void operator() (int& x)
    {
        sum_ += x;
        x = sum_;
    }

private:
    int sum_ = 0;
};


void test_for_each()
{
    cout << endl << "TEST FOREACH" << endl;
    int ints[15];
    my_stl_funcs::generate(ints, ints + 15, simple_functor_range(3));
    my_stl_funcs::for_each(ints, ints + 15, simple_functor_sum_scan());

    for (int i = 0; i < 15; i++)
    {
        cout << ints[i] << " ";
    }
    cout << endl;

    std::string strings[10];
    my_stl_funcs::fill<std::string>(strings, strings + 3, "abc");
    my_stl_funcs::fill<std::string>(strings + 6, strings + 10, "hello");
    
    auto func_concat_qqq = [] (std::string &s) { s += "qqq"; };
    my_stl_funcs::for_each(strings, strings + 10, func_concat_qqq);
    for (int i = 0; i < 10; i++)
    {
        cout << strings[i] << " ";
    }
    cout << endl;
}


void test_fifo()
{
    cout << endl << "TEST FIFO" << endl;

    fifo<std::string> x;
    x.push("abc");
    cout << x.size() << " " << x.front() << endl;

    fifo<std::string> y;
    y = x;
    cout << y.size() << " " << y.front() << endl;

    x.push("qqq");
    x.push("qqq");
    x.push("qqq");
    x.push("qqq");
    cout << x.size() << endl;
    cout << x.front() << endl;
    x.pop();
    cout << x.size() << endl;
    cout << x.front() << endl;
}

void test_priority()
{
    priority_queue<int> q;
    for (int i = 10; i >= 0; i--)
        q.push(i);

    for (int i = 0; i <= 10; i++)
    {
        cout << q.front() << " ";
        q.pop();
        if (i == 5)
            q.push(2);
    }
    cout << endl;

}

} // anonymous namespace

int main()
{
    test_fill();
    test_generate();
    test_copy();
    test_for_each();
    test_fifo();
    test_priority();
    return 0;
}
