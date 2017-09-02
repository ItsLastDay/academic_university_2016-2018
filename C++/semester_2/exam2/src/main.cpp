#include <iostream>
#include <cassert>
#include <vector>
#include <array>
#include <string.h>
#include <algorithm>
#include <list>
#include <iterator>
#include <set>

#include "au_allocator.h"

#define TASK1
#define TASK2
//#define TASK3

static void test_unckecked_allocation()
{
#ifdef TASK1
    au_allocator allocator;
    std::set<void*> allocs;

    const size_t count = 2000;
    for (size_t i = 0; i < count; ++i) 
    {
        allocs.insert(allocator.allocate(sizeof(double)));
    }
    assert(allocs.size() == count);
    
    for (void *ptr: allocs)
    {
        allocator.deallocate(ptr, sizeof(double));
    }
#endif
}

static void test_large_allocation()
{
#ifdef TASK1
    au_allocator allocator;
    void *ptr = allocator.allocate(1 << 10);
    allocator.deallocate(ptr, 1 << 10);
#endif
}

static void test_constructor_forwarding()
{
#ifdef TASK2
    struct test_struct
    {
        test_struct(int &&a, int &b, const int &c, int d)
            : a_(a)
            , b_(b)
            , c_(c)
            , d_(d)
        {}
        int a_; int b_; int c_; int d_;
    };
    au_allocator alloc;
    int b = 2;
    test_struct *ptr = alloc.allocate<test_struct>(1, b, 3, 4);
    assert(ptr->a_ == 1);
    assert(ptr->b_ == 2);
    assert(ptr->c_ == 3);
    assert(ptr->d_ == 4);
    alloc.deallocate(ptr);
#endif
}

static void test_destructor_call()
{
#ifdef TASK2
    struct test_struct
    {
        test_struct(int *val) : val_(val) {}
        ~test_struct() { *val_ = 0xDEAD; }
        int *val_;
    };
    int val = 0;
    au_allocator alloc;
    test_struct *ptr = alloc.allocate<test_struct>(&val);
    alloc.deallocate(ptr);
    assert(val == 0xDEAD);
#endif
}

static void test_stress()
{
#ifdef TASK2
    typedef size_t alloc1_t;
    typedef std::array<char, 10> alloc2_t;
    typedef std::array<char, 64> alloc3_t;
    typedef std::array<char, 1024> alloc4_t;

    std::vector<alloc1_t*> allocs1;
    std::vector<alloc2_t*> allocs2;
    std::vector<alloc3_t*> allocs3;
    std::vector<alloc4_t*> allocs4;
    au_allocator alloc;
    for(size_t i = 0; i < 2000; ++i)
    {
        alloc1_t *ptr1 = alloc.allocate<alloc1_t>();
        assert(ptr1);
        allocs1.push_back(ptr1);
        memset(allocs1.back(), 0x11, sizeof(alloc1_t));

        alloc2_t *ptr2 = alloc.allocate<alloc2_t>();
        assert(ptr2);
        allocs2.push_back(ptr2);
        memset(allocs2.back(), 0x22, sizeof(alloc2_t));

        alloc3_t *ptr3 = alloc.allocate<alloc3_t>();
        assert(ptr3);
        allocs3.push_back(ptr3);
        memset(allocs3.back(), 0x33, sizeof(alloc3_t));

        alloc4_t *ptr4 = alloc.allocate<alloc4_t>();
        assert(ptr4);
        allocs4.push_back(ptr4);
        memset(allocs4.back(), 0x44, sizeof(alloc4_t));
    }
    std::for_each(allocs1.begin(), allocs1.end(), [](alloc1_t *ptr) {
        for (char * p = (char*)ptr; p < (char*)(ptr + 1); ++p)
        assert(*p == 0x11);
    });
    std::for_each(allocs2.begin(), allocs2.end(), [](alloc2_t *ptr) {
        for (char * p = (char*)ptr; p < (char*)(ptr + 1); ++p)
        assert(*p == 0x22);
    });
    std::for_each(allocs3.begin(), allocs3.end(), [](alloc3_t *ptr) {
        for (char * p = (char*)ptr; p < (char*)(ptr + 1); ++p)
        assert(*p == 0x33);
    });
    std::for_each(allocs4.begin(), allocs4.end(), [](alloc4_t *ptr) {
        for (char * p = (char*)ptr; p < (char*)(ptr + 1); ++p)
        assert(*p == 0x44);
    });

    assert(!allocs1.empty());
    assert(!allocs2.empty());
    assert(!allocs3.empty());
    assert(!allocs4.empty());

    std::for_each(allocs4.begin(), allocs4.end(), [&alloc](alloc4_t *ptr) {
        alloc.deallocate(ptr);
    });
    std::for_each(allocs3.begin(), allocs3.end(), [&alloc](alloc3_t *ptr) {
        alloc.deallocate(ptr);
    });
    std::for_each(allocs2.begin(), allocs2.end(), [&alloc](alloc2_t *ptr) {
        alloc.deallocate(ptr);
    });
    std::for_each(allocs1.begin(), allocs1.end(), [&alloc](alloc1_t *ptr) {
        alloc.deallocate(ptr);
    });
#endif
}

static void test_allignment()
{
#ifdef TASK3

    struct alignas(64) helper
    {
        char data[31];
    };

    std::vector<helper*> ptrs;
    au_allocator alloc(8);

    for (size_t i = 0; i < 2000; ++i)
    {
        ptrs.push_back(alloc.allocate<helper>());
        assert((size_t)ptrs.back() % 64 == 0);
    }

    for (helper *ptr: ptrs)
    {
        alloc.deallocate(ptr);
    }

#endif  
}

int main()
{
    test_unckecked_allocation();
    test_large_allocation();
    test_constructor_forwarding();
    test_destructor_call();
    test_stress();
    test_allignment();
    return 0;
}
