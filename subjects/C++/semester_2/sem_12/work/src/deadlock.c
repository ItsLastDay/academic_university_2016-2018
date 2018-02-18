#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

pthread_mutex_t m1, m2;

void* func(void* thr_id)
{
    int id = *(int*)thr_id;
    printf("Entering thread %d\n", id);

    if (id == 1) 
    {
        pthread_mutex_lock(&m1);
        usleep(300);
        pthread_mutex_lock(&m2);
    }
    else
    {
        pthread_mutex_lock(&m2);
        pthread_mutex_lock(&m1);
    }
    return NULL;
}

int main()
{
    pthread_attr_t thread_attrs;
    pthread_attr_init(&thread_attrs);
    pthread_t *threads = (pthread_t*)malloc(sizeof(pthread_t) * 2);

    int id1 = 1;
    int id2 = 2;
    pthread_create(threads, &thread_attrs, &func, &id1);
    pthread_create(threads + 1, &thread_attrs, &func, &id2);

    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);
    return 0;
}
