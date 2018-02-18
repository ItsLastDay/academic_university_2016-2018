#pragma once

#include <iostream>

namespace my_pointers
{

template<class T>
struct unique_ptr
{
    using element_type = T;

    unique_ptr(unique_ptr const&) = delete;

    unique_ptr& operator=(unique_ptr && other_ptr) 
    {
        std::cout << "move assignment " << std::endl;
        std::cout << other_ptr.get() << std::endl;
        std::cout << pnt_ << std::endl;
        if (pnt_ == other_ptr.pnt_)
        {
            return *this;
        }
        delete pnt_;
        pnt_ = other_ptr.pnt_;
        other_ptr.pnt_ = nullptr;
        return *this;
    }

    unique_ptr(unique_ptr && other_ptr) 
    {
        std::cout << "move constructor " << std::endl;
        pnt_ = other_ptr.pnt_;
        other_ptr.pnt_ = nullptr;
        std::cout << pnt_ << std::endl;
    }


    explicit operator bool() const
    {
        return pnt_ != nullptr;
    }

    void reset(T* new_pnt=nullptr)
    {
        delete pnt_;
        pnt_ = new_pnt;
    }

    unique_ptr(T* pnt=nullptr)
        : pnt_(pnt)
    {
    }

    ~unique_ptr()
    {
        delete pnt_;
    }

    T const* get() const
    {
        return pnt_;
    }

    T& operator*()
    {
        return *pnt_;
    }

    T* operator->()
    {
        return pnt_;
    }

private:

    T* pnt_;
};

} // namespace my_pointers
