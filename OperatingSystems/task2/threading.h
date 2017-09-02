#ifndef __THREADING_H__
#define __THREADING_H__

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

enum thread_state
{
    RUNNING,
    FINISHED,
    DETACHED,
}

typedef struct __thread_t
{
    uint32_t id;
    thread_state state;

    size_t stack_size;
    void* stack_base;
    void* stack_top;

    struct __thread_t* waiting_for_me;
} thread_t;

// https://computing.llnl.gov/tutorials/pthreads/#PthreadsAPI

// https://computing.llnl.gov/tutorials/pthreads/man/pthread_create.txt
void thread_create(thread_t* thread, void* (*start_routine)(void*), void*);

// https://computing.llnl.gov/tutorials/pthreads/man/pthread_exit.txt
void thread_exit(void* value_ptr);

// https://computing.llnl.gov/tutorials/pthreads/man/pthread_cancel.txt
void thread_cancel(thread_t thread);

// https://computing.llnl.gov/tutorials/pthreads/man/pthread_join.txt
void thread_join(thread_t thread, void** value_ptr);


void threading_init(void);
thread_t thread_self(void);

#endif
