#include <map>
#include <set>
#include <unordered_set>
#include <vector>
#include <algorithm>

#include <cstdio>
#include <cstdint>

#include <pthread.h>
#include <dlfcn.h>


#define TASK3
#define TASK4

// Using `unordered_set` here, so that calling `s.erase` does not deallocate memory
// (deallocation causes segfaults).
std::map<pthread_t, std::unordered_set<pthread_mutex_t*>> thread_to_held_mutexes;

#ifdef TASK4
std::map<pthread_t, int> thread_to_mutexorder_lock, thread_to_mutexorder_release;
// I assume that every mutex is taken exactly once => order can be determined easily.
std::vector<pthread_mutex_t*> mutex_order;
#endif


extern "C" 
{
    using pthread_func_t = int (*)(pthread_mutex_t*);

    pthread_func_t real_mutex_lock;
    pthread_func_t real_mutex_unlock;


    __attribute__ ((constructor))
    static void my_constructor(void) noexcept
    {
        real_mutex_lock = reinterpret_cast<pthread_func_t>(
                dlsym(RTLD_NEXT, "pthread_mutex_lock"));
        real_mutex_unlock = reinterpret_cast<pthread_func_t>(
                dlsym(RTLD_NEXT, "pthread_mutex_unlock"));
    }

    __attribute__ ((destructor))
    static void my_destructor(void) noexcept
    {
    }

#ifdef TASK4
    int add_order(pthread_mutex_t* mutex) noexcept
    {
        auto it = std::find(mutex_order.begin(), mutex_order.end(), mutex);
        if (it == mutex_order.end())
        {
            mutex_order.push_back(mutex);
            return mutex_order.size() - 1;
        }

        return it - mutex_order.begin();
    }
#endif // TASK4

    int pthread_mutex_lock(pthread_mutex_t* mutex) noexcept
    {
        pthread_t cur_thread_id = pthread_self();
        if (thread_to_held_mutexes[cur_thread_id].count(mutex)) 
        {
            fprintf(stderr, "Thread %lx takes mutex %lx again\n", cur_thread_id, (uint64_t)mutex);
        }

#ifdef TASK3
        for (auto const& thread_set_pair: thread_to_held_mutexes)
        {
            if (thread_set_pair.second.count(mutex) && thread_set_pair.first != cur_thread_id) 
            {
                bool is_waiting_for_us = false;
                for (const auto& other_mutex: thread_set_pair.second)
                {
                    is_waiting_for_us |= thread_to_held_mutexes[cur_thread_id].count(other_mutex);
                }

                if (is_waiting_for_us)
                {
                    fprintf(stderr, "Thread %lx tries to lock mutex %lx, whereas thread %lx has this mutex and waits for one of the %lx-s mutexes\n",
                            cur_thread_id, (uint64_t)mutex, thread_set_pair.first, cur_thread_id);
                }
            }
        }
#endif // TASK3

#ifdef TASK4
        int idx_in_order = add_order(mutex);
        int next_mutex_to_lock = thread_to_mutexorder_lock[cur_thread_id];
        if (next_mutex_to_lock != idx_in_order)
        {
            fprintf(stderr, "Thread %lx tries to get mutex %lx, which is out of order\n", 
                    cur_thread_id, (uint64_t)mutex);
        }
        thread_to_mutexorder_lock[cur_thread_id]++;
#endif // TASK4

        thread_to_held_mutexes[cur_thread_id].insert(mutex);
        fprintf(stdout, "Mutex at %lx taken by %lx\n", (uint64_t)mutex, cur_thread_id);
        return real_mutex_lock(mutex);
    }

    int pthread_mutex_unlock(pthread_mutex_t* mutex) noexcept
    {
        pthread_t cur_thread_id = pthread_self();
        thread_to_held_mutexes[cur_thread_id].erase(mutex);

#ifdef TASK4
        int idx_in_order = add_order(mutex);
        int next_mutex_to_unlock = thread_to_mutexorder_release[cur_thread_id];
        if (next_mutex_to_unlock != idx_in_order)
        {
            fprintf(stderr, "Thread %lx is unlocking mutex %lx, which is out of order\n",
                    cur_thread_id, (uint64_t)mutex);
        }
        thread_to_mutexorder_release[cur_thread_id]++;
#endif // TASK4

        fprintf(stdout, "Mutex at %lx released\n", (uint64_t)mutex);
        return real_mutex_unlock(mutex);
    }
}

