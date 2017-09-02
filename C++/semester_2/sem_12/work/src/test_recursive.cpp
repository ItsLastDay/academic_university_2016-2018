#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

const size_t THREADS_COUNT = 3;
pthread_mutex_t shared_mutex = PTHREAD_MUTEX_INITIALIZER;

void* worker_func(void *ignored)
{
    (void)ignored;
    pthread_mutex_lock(&shared_mutex);
    pthread_mutex_lock(&shared_mutex);
    pthread_mutex_unlock(&shared_mutex);
    return NULL;
}

int main()
{
    pthread_attr_t thread_attrs;
    pthread_attr_init(&thread_attrs);
    pthread_t threads[THREADS_COUNT];

    for(size_t workers_ix = 0; workers_ix < THREADS_COUNT; ++workers_ix)
    {
        pthread_create(&threads[workers_ix], &thread_attrs,
                &worker_func, NULL);
    }

    for(size_t workers_ix = 0; workers_ix < THREADS_COUNT; ++workers_ix)
    {
        pthread_join(threads[workers_ix], NULL);
    }

    return EXIT_SUCCESS;
}
