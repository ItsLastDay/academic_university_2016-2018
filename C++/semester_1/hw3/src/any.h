#pragma once

#include <stdexcept>

#include <typeinfo>
#include <type_traits>


namespace utils
{

struct bad_any_cast
    : std::runtime_error
{
    bad_any_cast(const char* descr)
        : std::runtime_error(descr)
    {
    }
};


struct any
{
    template<class T>
    any(T const& obj)
        : obj_(new any_holder<T>(obj))
    {
    }

    any();
    ~any();

    any(any const& other);
    void swap(any& other);
    any& operator=(any other);

    bool empty() const;
    const std::type_info& type() const;

    template<class T> friend T* any_cast(any*);
    template<class T> friend T any_cast(any&);
private:
    struct any_holder_base
    {
        virtual ~any_holder_base()
        {
        }

        virtual any_holder_base* clone() const = 0;
        virtual const std::type_info& type() const = 0;
    };

    template<class T>
    struct any_holder 
        : any_holder_base
    {
        any_holder() = delete;
        any_holder& operator=(any_holder) = delete;

        any_holder(T const& obj)
            : typed_obj_(obj)
        {
        }

        any_holder_base* clone() const override
        {
            return new any_holder(typed_obj_);
        }

        const std::type_info& type() const override
        {
            return typeid(T);
        }

        T typed_obj_;
    };


    any_holder_base* obj_;
};


template<class T>
T* any_cast(any* value)
{
    auto const& held_type = value->type();
    if (held_type != typeid(T))
    {
        return nullptr;
    }

    auto holder = static_cast<any::any_holder<T>*>(value->obj_);
    return &(holder->typed_obj_);
}


template<class T>
T const* any_cast(any const* value)
{
    // Since I do not try to assign to `value` in `any_cast`,
    // `const_cast` should be ok.
    return any_cast<T>(const_cast<any*>(value));
}


template<class T>
T any_cast(any& value)
{
    auto const& held_type = value.type();
    if (held_type != typeid(T))
    {
        std::string err_msg = "bad_any_cast: could not cast ";
        err_msg += held_type.name();
        err_msg += " to ";
        err_msg += typeid(T).name();
        throw bad_any_cast(err_msg.c_str());
    }

    // `remove_reference` is used, because it is meaningless to have an instance
    // of, for example, `any_holder<int&>` -- it is really a `any_holder<int>`.
    // But template parameter `T` can be `int&`. So, without `remove_reference`
    // this code would attempt to cast to `any_holder<int&>` and nothing works.
    auto holder = static_cast<any::any_holder<typename std::remove_reference<T>::type>*>(value.obj_);
    return holder->typed_obj_;
}

template<class T>
T any_cast(any const& value)
{
    typedef typename std::remove_reference<T>::type nonref;
    return any_cast<const nonref &>(const_cast<any&>(value));
}


void swap(any& x, any& y);

} // namespace utils
