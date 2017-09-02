#include "safe_dl.h"

using namespace safe_dl;

#ifndef SHARED2

int identity(int x)
{
    return 0; //Note: it's wrong identity
}

#else

int identity(int x)
{
    return x;
}

#endif

int many_args(int, float, double, short)
{
    return 42;
}

#ifndef TASK3

registrator register_identity ("identity", identity);
registrator register_many_args("many_args", many_args);

#else

REG_FUNCTION(identity);
REG_FUNCTION(many_args);

#endif