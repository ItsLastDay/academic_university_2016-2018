#include <utility>
#include <ciso646>
#include <exception>
#include <vector>
#include <thread>
#include <cstddef>
#include <queue>
#include <functional>

#include "thread_pool.hpp"

namespace au
{

// Code influenced by
// https://channel9.msdn.com/Shows/Going+Deep/C-and-Beyond-2012-Herb-Sutter-Concurrency-and-Parallelism

struct concurrent_task_queue
{
    using value_type = std::function<void()>;
    using pointer_type = std::shared_ptr<value_type>;

    concurrent_task_queue(size_t max_size)
        : max_size_(max_size)
    {
    }

    concurrent_task_queue(concurrent_task_queue const&) = delete;
    concurrent_task_queue(concurrent_task_queue &&) = delete;

    concurrent_task_queue& operator=(concurrent_task_queue const&) = delete;
    concurrent_task_queue& operator=(concurrent_task_queue &&) = delete;

    void push(value_type &&func)
    {
        while (true)
        {
            while (task_queue_.size() >= max_size_)
            {
            }
            std::lock_guard<std::mutex> lock(queue_mutex_);
            if (task_queue_.size() < max_size_)
            {
                task_queue_.push(std::make_shared<value_type>(std::move(func)));
                break;
            }
        }
    }


    std::pair<pointer_type, bool> try_pop()
    {
        if (not task_queue_.empty())
        {
            std::lock_guard<std::mutex> lock(queue_mutex_);
            if (not task_queue_.empty())
            {
                auto top = task_queue_.front();
                task_queue_.pop();
                return {top, true};
            }
        }
        return {nullptr, false};
    }

    size_t max_queue_size() const
    {
        return max_size_;
    }

    size_t size() const
    {
        std::lock_guard<std::mutex> lock(queue_mutex_);
        return task_queue_.size();
    }

    void set_max_queue_size(size_t new_queue_size)
    {
        while (true)
        {
            while (task_queue_.size() >= new_queue_size)
            {
            }
            std::lock_guard<std::mutex> lock(queue_mutex_);
            if (task_queue_.size() < new_queue_size)
            {
                max_size_ = new_queue_size;
                break;
            }
        }
    }

private:
    using stored_type = pointer_type;

    mutable std::mutex queue_mutex_;
    size_t max_size_;
    std::queue<stored_type> task_queue_;
};


struct queued_worker
{
    queued_worker(concurrent_task_queue& q)
        : done_(false)
        , task_queue_(q)
        , thr_([this] {
                while (not done_) 
                {
                    auto task_avail = task_queue_.try_pop(); 
                    if (task_avail.second)
                    {
                        (*task_avail.first)();
                    }
                }
        })
    {
    }

    queued_worker(queued_worker const&) = delete;
    queued_worker& operator=(queued_worker const&) = delete;

    queued_worker(queued_worker &&) = default;

    ~queued_worker()
    {
        done_ = true;
        thr_.join();
    }

private:
    bool done_;
    concurrent_task_queue& task_queue_;
    std::thread thr_;
};


thread_pool::thread_pool(size_t threads_count, size_t max_queue_size)
    : task_queue_(max_queue_size)
{
    set_threads_count(threads_count);
}


thread_pool::~thread_pool()
{
    while (task_queue_.size() > 0)
    {
        // Do nothing and wait.
    }
}

void thread_pool::set_threads_count(size_t threads_count)
{
    std::lock_guard<std::mutex> guard(workers_mutex_);
    while (available_workers_.size() < threads_count)
    {
        available_workers_.push_back(std::make_shared<detail::queued_worker>(task_queue_));
    }

    while (available_workers_.size() > threads_count)
    {
        available_workers_.pop_back();
    }
}


void thread_pool::set_max_queue_size(size_t max_queue_size)
{
    task_queue_.set_max_queue_size(max_queue_size);
}

size_t thread_pool::threads_count() const
{
    std::lock_guard<std::mutex> guard(workers_mutex_);
    return available_workers_.size();
}

size_t thread_pool::max_queue_size() const
{
    return task_queue_.max_queue_size();
}
