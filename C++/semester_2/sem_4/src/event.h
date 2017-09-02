#pragma once
#include <functional>
#include <vector>


namespace utils
{

template<class>
struct event;


template<class R, class... Args>
struct event<R(Args...)>
{
    using callback_t = std::function<R(Args...)>;
    using callback_container = std::vector<callback_t>;

    struct event_deleter
    {
        event_deleter(callback_container &callback_cont,
                typename callback_container::size_type idx)
            : _callbacks_ref(callback_cont)
            , _idx(idx)
        {
        }

        void operator()()
        {
            _callbacks_ref[_idx] = nullptr;
        }

    private:
        callback_container &_callbacks_ref;
        typename callback_container::size_type _idx;
    };

    event() = default;
    event(event &&x) = default;
    event& operator=(event &&x) = default;

    event(event &x) = delete;
    event& operator=(event &x) = delete;

    void fire(Args... args)
    {
        for (auto const& callback: _callbacks)
        {
            if (callback) 
            {
                callback(args...);
            }
        }
    }

    std::function<void()> add(callback_t callback)
    {
        // We already have a temporary object - a "callback_t",
        // which is std::function. We need to move it,
        // regardless of what was used for its construction.
        _callbacks.push_back(std::move(callback));
        return event_deleter(_callbacks, _callbacks.size() - 1);
    }

private:
    callback_container _callbacks;
};


} // namespace utils.
