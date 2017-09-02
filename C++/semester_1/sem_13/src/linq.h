#pragma once

#include <vector>
#include <functional>
#include <iostream>


namespace linq
{

template<class T>
struct enumerable;

template<class T>
enumerable<T> from(std::vector<T> const& vec);


template<class T>
struct enumerable
{
    enumerable(std::vector<T> const& vec)
        : values_(vec)
    {
    }

    enumerable() = delete;
    enumerable& operator=(enumerable const&) = delete;

    ~enumerable() = default;

    enumerable where(std::function<bool(T)> func) const
    {
        std::vector<T> filtered_values;
        for (auto const& val: values_)
        {
            if (func(val))
            {
                filtered_values.push_back(val);
            }
        }
        return from(filtered_values);
    }

    std::vector<T> toVector() const
    {
        return values_;
    }


    template<class U>
    enumerable select(std::function<U(T)> func) const
    {
        std::vector<U> transformed_values;
        for (auto const& val: values_)
        {
            transformed_values.push_back(func(val));
        }
        return from<typename typeid(func)::result_type>(transformed_values);
    }


    size_t count() const
    {
        return values_.size();
    }


    size_t count(std::function<bool(T)> pred)
    {
        size_t cnt = 0;
        for (auto const& val: values_)
        {
            cnt += pred(val);
        }
        return cnt;
    }

    bool any() const
    {
        return count() > 0;
    }

    bool any(std::function<bool(T)> pred)
    {
        for (auto const& val: values_)
        {
            if (pred(val))
            {
                return true;
            }
        }
        return false;
    }

    enumerable print(std::ostream& out)
    {
        for (auto const& val: values_)
        {
            out << val << " ";
        }
        out << std::endl;

        return *this;
    }

private:

    std::vector<T> values_;
};

template<class T>
enumerable<T> from(std::vector<T> const& vec) 
{
    return enumerable<T>(vec);
}


} // namespace linq
