#include <cstdio>
#include <unistd.h>
#include <cstdlib>

static void use_unintialized_mem()
{
    int *ptr1 = new int(123);
    int *ptr2 = new int;
    printf("%d\n", *ptr1);
    printf("%d\n", *ptr2);
    delete ptr1;
    delete ptr2;
}

static void use_after_free()
{
    int *ptr1 = new int(123);
    delete ptr1;
    printf("%d\n", *ptr1);
}

static void dyn_mem_bounds_violate()
{
    int *int_array = new int[3] { 123, 123, 123 };
    printf("%d\n", int_array[-1]);
    printf("%d\n", int_array[3]);
    delete[] int_array;
}

static void mem_leak()
{
    new int;
}

static void sys_call_invalid_buf()
{
    int *ptr = new int;
    write(STDOUT_FILENO, reinterpret_cast<void*>(0xDEADBEAF), 100500);
    write(STDOUT_FILENO, static_cast<void*>(ptr), sizeof(*ptr));
    delete ptr;
}

static void invalid_frees()
{
    int *ptr = new int;
    delete ptr;
    delete ptr;
    free((void*)ptr);
    free((void*)0xDEADBEAF);
}

int main()
{
    freopen("/dev/null", "w", stdout);
    use_unintialized_mem();
    use_after_free();
    dyn_mem_bounds_violate();
    mem_leak();
    sys_call_invalid_buf();
    invalid_frees();
    return 0;
}
