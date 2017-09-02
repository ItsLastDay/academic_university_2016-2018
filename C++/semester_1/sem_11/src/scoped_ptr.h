#pragma once


namespace my_pointers
{

template<class T>
struct scoped_ptr
{
    using element_type = T;

    scoped_ptr& operator=(scoped_ptr) = delete;
    scoped_ptr(scoped_ptr const&) = delete;
    scoped_ptr(scoped_ptr &&) = delete;


    explicit operator bool() const
    {
        return pnt_ != nullptr;
    }

    void reset(T* new_pnt=nullptr)
    {
        delete pnt_;
        pnt_ = new_pnt;
    }

    scoped_ptr(T* pnt=nullptr)
        : pnt_(pnt)
    {
    }

    ~scoped_ptr()
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
