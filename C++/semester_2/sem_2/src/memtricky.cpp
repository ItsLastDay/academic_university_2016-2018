#include <cstdio>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>

void vector_bounds_violation()
{
    std::vector<int> vec; // = {1, 2, 3, 4, 5}
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    vec.push_back(4);
    vec.push_back(5);
    std::vector<int>::iterator it_prev_begin = vec.begin() - 1;
    for(std::vector<int>::iterator it = it_prev_begin; it <= vec.end(); ++it)
    {
        std::cout << *it << std::endl;
    }
}

void vector_iterator_invalidation()
{
    std::vector<int> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    vec.push_back(4);
    vec.push_back(5);
    for(auto it = vec.begin(); it != vec.end(); ++it) {
        std::cout << *it << std::endl;
        if (*it == 2) {
            vec.push_back(99);
        }
    }
}

void array_stack_bounds_violation()
{
    int ar[5];
    std::fill(ar - 1, ar + 1, 0xDEADBEAF);
}

static void memcpy_overlap()
{
    int vec[10];
    memcpy(&vec[0], &vec[2], sizeof(vec[0]) * 3);
}

void invalid_pointer_aliasing()
{
    int *iptr = new int(123);
    void *ptr = static_cast<void*>(iptr);
    double *dptr = static_cast<double*>(ptr);
    std::cout << dptr << std::endl;
    delete iptr;
}

void class_memcpy()
{
    struct foo
    {
        foo() {}
        int a; int b; std::vector<int> vec;
    };
    foo foo1;
    foo foo2;
    memcpy(&foo1, &foo2, sizeof(foo1));
}

int main()
{
    freopen("/dev/null", "w", stdout);
    vector_bounds_violation();
    vector_iterator_invalidation();
    array_stack_bounds_violation();
    memcpy_overlap();
    invalid_pointer_aliasing();
    class_memcpy();
    return 0;
}
