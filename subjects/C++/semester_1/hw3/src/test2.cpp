#include <iostream>

#ifdef MY_COMPUTER
#include "any.h"
using utils::any;
using utils::any_cast;
#else
#include <boost/any.hpp>
using boost::any;
using boost::any_cast;
#endif

#define GO(x) do {\
        bool thrown = false; \
        try \
        {\
            (x);\
        } catch(...)\
        {\
            thrown = true;\
        }\
        std::cout << thrown << std::endl;\
    }\
    while(false);

int main()
{
    int x = 5;
    any ref(x);
    GO(any_cast<int>(ref));
    GO(any_cast<const int>(ref));
    GO(any_cast<double>(ref));
    GO(any_cast<const double>(ref));
    GO(any_cast<int&>(ref));
    GO(any_cast<double&>(ref));
    GO(any_cast<const int&>(ref));
    GO(any_cast<const double&>(ref));
    GO(any_cast<int*>(ref));
    GO(any_cast<double*>(ref));
    GO(any_cast<const int*>(ref));
    GO(any_cast<const double*>(ref));


    x = 6;
    const any const_ref(x);
    GO(any_cast<int>(const_ref));
    GO(any_cast<const int>(const_ref));
    GO(any_cast<double>(const_ref));
    GO(any_cast<const double>(const_ref));
    //GO(any_cast<int&>(const_ref));
    //GO(any_cast<double&>(const_ref));
    GO(any_cast<const int&>(const_ref));
    GO(any_cast<const double&>(const_ref));
    GO(any_cast<int*>(const_ref));
    GO(any_cast<double*>(const_ref));
    GO(any_cast<const int*>(const_ref));
    GO(any_cast<const double*>(const_ref));


    x = 7;
    any pointer(x);
    GO(any_cast<int>(&pointer));
    GO(any_cast<const int>(&pointer));
    GO(any_cast<double>(&pointer));
    GO(any_cast<const double>(&pointer));
    //GO(any_cast<int&>(&pointer));
    //GO(any_cast<double&>(&pointer));
    GO(any_cast<const int&>(&pointer));
    GO(any_cast<const double&>(&pointer));
    GO(any_cast<int*>(&pointer));
    GO(any_cast<double*>(&pointer));
    GO(any_cast<const int*>(&pointer));
    GO(any_cast<const double*>(&pointer));


    x = 8;
    const any const_pointer(x);
    GO(any_cast<int>(&const_pointer));
    GO(any_cast<const int>(&const_pointer));
    GO(any_cast<double>(&const_pointer));
    GO(any_cast<const double>(&const_pointer));
    //GO(any_cast<int&>(&const_pointer));
    //GO(any_cast<double&>(&const_pointer));
    GO(any_cast<const int&>(&const_pointer));
    GO(any_cast<const double&>(&const_pointer));
    GO(any_cast<int*>(&const_pointer));
    GO(any_cast<double*>(&const_pointer));
    GO(any_cast<const int*>(&const_pointer));
    GO(any_cast<const double*>(&const_pointer));

    return 0;
}
