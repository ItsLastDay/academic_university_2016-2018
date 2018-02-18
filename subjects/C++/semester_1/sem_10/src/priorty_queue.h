#pragma once

#include <algorithm>
#include <vector>


template<class T>
struct priority_queue
{
    priority_queue() = default;
    priority_queue(priority_queue const&) = default;

    priority_queue& operator=(priority_queue other)
    {
        swap(other);
        return *this;
    }

    void swap(priority_queue &other)
    {
        std::swap(contents_, other.contents_);
    }

    void push(T const& val)
    {
        contents_.push_back(val);
        sift_up(contents_.size() - 1);
    }

    void pop()
    {
        std::swap(contents_[0], contents_.back());
        contents_.pop_back();
        heapify(0);     
    }

    T& front()
    {
        return contents_[0];
    }
    const T& front() const
    {
        return contents_[0];
    }


    size_t size() const
    {
        return contents_.size();
    }


    ~priority_queue() = default;
private:
    void sift_up(int idx)
    {
        while (idx > 0)
        {
            int par = get_par(idx);
            if (contents_[par] > contents_[idx])
            {
                std::swap(contents_[par], contents_[idx]);
                idx = par;
            }
            else
            {
                break;
            }
        }
    }

    void heapify(int idx)
    {
        int left_child_idx = get_left_child(idx);
        int right_child_idx = get_right_child(idx);
        int min_idx = idx;

        if (left_child_idx < static_cast<int>(size()) and contents_[left_child_idx]
                < contents_[min_idx])
        {
            min_idx = left_child_idx;
        }

        if (right_child_idx < static_cast<int>(size()) and contents_[right_child_idx]
                < contents_[min_idx])
        {
            min_idx = right_child_idx;
        }

        if (min_idx != idx)
        {
            std::swap(contents_[idx], contents_[min_idx]);
            heapify(min_idx);
        }
    }

    int get_par(int idx) const
    {
        return (idx - 1) / 2;
    }

    int get_left_child(int idx) const
    {
        return idx * 2 + 1;
    }   

    int get_right_child(int idx) const
    {
        return idx * 2 + 2;
    }

    std::vector<T> contents_;
};
