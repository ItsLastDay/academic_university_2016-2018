#include <cstdio>
#include <cstdint>

#include <pthread.h>
#include <dlfcn.h>


extern "C" 
{
    using pthread_func_t = int (*)(pthread_mutex_t*);

    pthread_func_t real_mutex_lock;
    pthread_func_t real_mutex_unlock;


    __attribute__ ((constructor))
    static void my_constructor(void)
    {
        real_mutex_lock = reinterpret_cast<pthread_func_t>(
                dlsym(RTLD_NEXT, "pthread_mutex_lock"));
        real_mutex_unlock = reinterpret_cast<pthread_func_t>(
                dlsym(RTLD_NEXT, "pthread_mutex_unlock"));
    }

    __attribute__ ((destructor))
    static void my_destructor(void)
    {
    }

    int pthread_mutex_lock(pthread_mutex_t* mutex) noexcept
    {
        fprintf(stdout, "Mutex at %lx taken\n", (uint64_t)mutex);
        return real_mutex_lock(mutex);
    }

    int pthread_mutex_unlock(pthread_mutex_t* mutex) noexcept
    {
        fprintf(stdout, "Mutex at %lx released\n", (uint64_t)mutex);
        return real_mutex_unlock(mutex);
    }
}

