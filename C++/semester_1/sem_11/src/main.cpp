#include "scoped_ptr.h"
#include "unique_ptr.h"

#include <iostream>

using std::cout;
using std::endl;

using namespace my_pointers;

namespace
{

struct foo
{
    foo(int a, int b)
        : field1(a)
        , b_(b)
    {
    }

    int field1, b_;
};


bool bool_first_arg_func(bool arg)
{
    return arg;
}


void bar()
{
    scoped_ptr<foo> foo_ptr(new foo(100, 1));

    cout << (*foo_ptr).field1 << " " << foo_ptr->field1 << " " <<
        foo_ptr.get() << endl;

    if (foo_ptr)
    {
        foo_ptr->field1 += 1;
    }

    cout << (*foo_ptr).field1 << " " << foo_ptr->field1 << " " <<
        foo_ptr.get() << endl;

//    foo_ptr = std::move(foo_ptr);
//    auto foo_ptr2(foo_ptr);
//    foo_ptr = foo_ptr;
//    bool_first_arg_func(foo_ptr);
    bool_first_arg_func(false);

    foo_ptr.reset();
//    cout << foo_ptr->field1 << endl;
    foo_ptr.reset(new foo(123, 45));
    cout << foo_ptr->field1 << endl;

    scoped_ptr<foo> foo_ptr2;
//    cout << foo_ptr2->field1 << endl;

    decltype(foo_ptr)::element_type foo_on_stack(11, 12);
    cout << foo_on_stack.field1 << endl;

    cout << "End of scope" << endl;
};

void unique_bar()
{
    unique_ptr<foo> foo_uptr(new foo(200, 2123));
    unique_ptr<foo> foo_uptr2 = std::move(foo_uptr);
    cout << "bla " << foo_uptr2->field1 << endl;

    foo_uptr2 = std::move(foo_uptr2);

    cout << "Here" << endl;
    cout << "bla " << foo_uptr2.get() << endl;
    cout << foo_uptr2->field1 << endl;
}


} // anon namespace


int main()
{
    bar();
    unique_bar();
    return 0;
}
