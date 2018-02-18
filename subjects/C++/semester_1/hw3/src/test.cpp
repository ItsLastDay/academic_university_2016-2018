#include <cassert>
#include <memory>
#include <string>
#include <iostream>

#include "any.h"

using utils::any;
using utils::any_cast;
using utils::bad_any_cast;

using std::string;
using std::cout;
using std::cerr;
using std::endl;



void contruct_test()
{
    any def;
    any copy_on_type(42);

    def = 3.14;
    def = string("2.71");

    any def_copy(def);
    def = copy_on_type;

    any e;
    assert(e.empty());
    cout << "Construct test passed" << endl;
}

template<class T>
void check_cast(any &a, bool should_throw)
{
    bool thrown = false;
    try
    {
        double res = any_cast<T>(a);
        std::cout << res;
    }
    catch(bad_any_cast const& err)
    {
        thrown = true;
        std::cerr << err.what() << std::endl;
    }
    assert(should_throw == thrown);
}


template<class T>
void check_cast(any const& a, bool should_throw)
{
    cout << "const check cast" << endl;
    bool thrown = false;
    try
    {
        double res = any_cast<T>(a);
        std::cout << res;
    }
    catch(bad_any_cast const& err)
    {
        thrown = true;
        std::cerr << err.what() << std::endl;
    }
    assert(should_throw == thrown);
}


void retrieve_value_test()
{
    any ia(42);
    // Casting to a pointer.
    auto res = any_cast<double>(&ia);
    any const* tt = &ia;
    auto res_int = any_cast<int>(tt);
    assert(*res_int == 42);
    assert(res == nullptr);
    check_cast<double>(ia, true );
    check_cast<int> (ia, false);

    // Casting to a const pointer.
    any istr(std::string("abacaba"));
    any const* ptr = &istr;
    std::string const* str = any_cast<std::string>(ptr);
    assert(*str == "abacaba");
    try
    {
        cout << any_cast<const char*>(istr) << endl;
        assert(false);
    }
    catch(bad_any_cast const& exc)
    {
    }

    // Casting to a reference and const reference.
    int& x = any_cast<int&>(ia);
    assert(x == 42);
    x = 30;
    int const& y = any_cast<int const&>(ia);
    assert(y == 30);
    try
    {
        cout << any_cast<double&>(ia) << endl;
        assert(false);
    }
    catch(bad_any_cast const& exc)
    {
    }


    cout << "Retrieve test passed" << endl;
}


void const_ref_test()
{
    cout << "Const reference test" << endl;
    any ia(42);
    const any iac(ia);
    //check_cast<double&>(iac, true);
    //check_cast<int&>(iac, true);
    cout << "Const reference test passed" << endl;
}


void swap_test(any& a, any& b)
{
    swap(a, b);
}

int main()
{
    contruct_test();
    retrieve_value_test();
    const_ref_test();

    any a(5), b(string("6"));
    swap_test(a, b);
    assert(any_cast<int>(b) == 5);
    assert(any_cast<string&>(a) == "6");

    any empty;
    any str(std::string("foobar"));
    empty = str;
    assert(any_cast<string&>(empty) == any_cast<string&>(str));

    const any aa(234);
    any_cast<int>(aa);
    any_cast<const int>(aa);

    any ia(42);
    check_cast<int&> (ia, false);
    check_cast<const int&> (ia, false);


    const any iac(42);
    check_cast<const int&> (iac, false);

    any a_destroy(std::make_shared<int>(0));
        

    cout << "Main finished" << endl;
    return 0;
}
