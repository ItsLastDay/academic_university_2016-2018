#pragma once

#include <cassert>
#include <algorithm>
#include <unordered_map>
#include <memory>
#include <list>
#include <cstdint>
#include <vector>
#include <stdexcept>



namespace detail
{

namespace
{
    const int N = 4096;
} // anonymous namespace 

struct chunk
{
    chunk(size_t order)
        : order_(order)
        , num_used_(0)
    {
        for (size_t i = 0; i < get_num_elems(); i++)
        {
            free_indices_.push_back(static_cast<uint16_t>(i));
        }
    }

    ~chunk() = default;

    chunk(chunk const&) = delete;
    chunk(chunk &&) = delete;
    chunk& operator=(chunk const&) = delete;
    chunk& operator=(chunk &&) = delete;


    bool is_empty() const
    {
        return num_used_ == 0;
    }

    bool is_full() const
    {
        return num_used_ == get_num_elems();
    }

    bool will_full_after_insert() const
    {
        return num_used_ + 1 == get_num_elems();
    }

    void* get_mem() noexcept
    {
        size_t i = *free_indices_.begin();
        free_indices_.pop_front();
        num_used_++;
        return (void*)(buf_ + i * (1ull << order_));
    }

    void reclaim(void* ptr)
    {
        char* cptr = static_cast<char*>(ptr);
        size_t idx = (cptr - buf_) / (1ull << order_);
        free_indices_.push_back(idx); // can throw
        num_used_--;
    }

private:
    size_t get_num_elems() const noexcept
    {
        return N / (1ull << order_);
    }

    size_t order_;
    char buf_[N];
    std::list<uint16_t> free_indices_;
    size_t num_used_;
};

struct order_allocator
{
    explicit order_allocator(size_t order)
        : order_(order)
    {
    }

    ~order_allocator() = default;
    order_allocator(order_allocator &&) = default;

    order_allocator(order_allocator const&) = delete;
    order_allocator& operator=(order_allocator const&) = delete;
    order_allocator& operator=(order_allocator &&) = delete;

    void drop_unused()
    {
        for (auto it = free_.begin(); it != free_.end(); )
        {
            if ((*it)->is_empty())
            {
                it = free_.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }

    void* get_mem()
    {
        if (free_.empty())
        {
            // Can throw.
            free_.push_back(std::make_unique<chunk>(order_));
        }

        void* ptr;
        auto it = free_.begin();

        if ((*free_.begin())->will_full_after_insert())
        {
            // Perform possibly throwing operations first.
            occupied_.push_front(std::move(*free_.begin()));
            free_.pop_front(); 
            it = occupied_.begin();
        }

        ptr = (*it)->get_mem();
        ptr_to_chunk[ptr] = it->get();

        return ptr;
    }

    void reclaim(void* ptr)
    {
        chunk* chunk_ptr = ptr_to_chunk[ptr];
        bool was_full = chunk_ptr->is_full();

        // Move from 'occupied' to 'free' if chunk was
        // full, because it will be not full after reclaiming.
        if (was_full)
        {
            auto it = std::find_if(occupied_.begin(),
                    occupied_.end(),
                    [chunk_ptr](auto const& stored_ptr)
                    {
                        return stored_ptr.get() == chunk_ptr;
                    });
            free_.push_back(std::move(*it)); // can throw
            occupied_.erase(it);
        }

        chunk_ptr->reclaim(ptr);
	ptr_to_chunk.erase(ptr);
    }

private:
    size_t order_;

    std::unordered_map<void*, chunk*> ptr_to_chunk;

    // All cells occupied
    std::list<std::unique_ptr<chunk>> occupied_;
    // Some cells free
    std::list<std::unique_ptr<chunk>> free_;
};

} // namespace detail

struct au_allocator_ex
    : std::runtime_error
{
    using std::runtime_error::runtime_error;
};

struct au_allocator
{
    explicit au_allocator(size_t max_order=7)
        : max_order_(max_order)
    {
        if (detail::N < (1ull << (max_order - 1)))
        {
            throw new au_allocator_ex("Incorrect max_order");
        }

        for (size_t i = 0; i < max_order; i++)
        {
            allocators_.emplace_back(i);
        }
    }

    ~au_allocator() = default;

    au_allocator(au_allocator const&) = delete;
    au_allocator(au_allocator&&) = delete;
    au_allocator& operator=(au_allocator const&) = delete;
    au_allocator& operator=(au_allocator&&) = delete;

    void drop_unused()
    {
        for (auto& allocator: allocators_)
        {
            allocator.drop_unused();
        }
    }

    void* allocate(size_t size)
    {
        auto order = get_order_from_size(size);

        if (order >= max_order_)
        {
            return new char[size];
        }
        return allocators_[order].get_mem();
    }

    void deallocate(void* ptr, size_t size)
    {
        auto order = get_order_from_size(size);
        if (order >= max_order_)
        {
            delete[] static_cast<char*>(ptr);
            return;
        }

        allocators_[order].reclaim(ptr);
    }

    template<class T, class... Args>
    T* allocate(Args&&... args)
    {
        auto ptr = allocate(sizeof(T));
        try
        {
            new (ptr) T(std::forward<Args>(args)...);
        }
        catch(...)
        {
            deallocate(ptr, sizeof(T));
            throw;
        }

        return static_cast<T*>(ptr);
    }

    template<class T>
    void deallocate(T* const ptr)
    {
        ptr->~T();
        deallocate(static_cast<void*>(ptr), sizeof(*ptr));
    }

private:
    size_t get_order_from_size(size_t size)
    {
        size_t order = 0;
        while (size > (1ull << order))
        {
            ++order;
        }
        return order;
    }

    size_t max_order_;
    std::vector<detail::order_allocator> allocators_;
};
