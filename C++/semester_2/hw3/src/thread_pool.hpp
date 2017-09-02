#pragma once

#include <vector>
#include <memory>
#include <functional>
#include <future>

namespace au
{

struct thread_pool
{
    thread_pool& operator=(thread_pool const&) = delete;
    thread_pool& operator=(thread_pool &&) = delete;

    thread_pool(thread_pool const&) = delete;
    thread_pool(thread_pool &&) = delete;

    thread_pool(size_t threads_count, size_t max_queue_size);
    ~thread_pool();

    template<class Function, class... Args>
    auto submit(Function&& function, Args&&... args) 
    {
        auto fn = std::bind(std::forward<Function>(function), std::forward<Args>(args)...);
        using func_ret = decltype(fn());
        auto task = std::make_shared<std::packaged_task<func_ret()>>(std::move(fn));
        auto returned_future = task->get_future();

        task_queue_.push([task] {
            (*task)();
        });

        return returned_future;
    }

    void set_threads_count(size_t threads_count);

    void set_max_queue_size(size_t max_queue_size);

    size_t threads_count() const;

    size_t max_queue_size() const;

private:
    detail::concurrent_task_queue task_queue_;
    std::vector<std::shared_ptr<detail::queued_worker>> available_workers_;
    mutable std::mutex workers_mutex_;
};

} // namespace au
