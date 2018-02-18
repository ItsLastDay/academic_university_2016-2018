#pragma once

#include <functional>
#include <memory>
#include <vector>



namespace utils
{

template<class>
struct signal;


template<class R, class... Args>
struct signal<R(Args...)>
{
private:
    using callback_t = std::function<R(Args...)>;
    using callback_container_t = std::vector<callback_t>;

public:
    struct connection
    {
        connection(connection &&) = default;
        connection & operator=(connection&& ) = default;

        connection() = delete;
        connection(connection &) = delete;
        connection& operator=(connection &) = delete;

        connection(std::shared_ptr<callback_container_t> cont_ptr,
                typename callback_container_t::size_type idx)
            : owning_container_ptr_(cont_ptr)
            , idx_(idx)
        {
        }

        void disconnect()
        {
            if (owning_container_ptr_.expired())
            {
                return;
            }

            owning_container_ptr_.lock()->at(idx_) = nullptr;
        }

    private:
        std::weak_ptr<callback_container_t> owning_container_ptr_;
        typename callback_container_t::size_type idx_;
    };


    struct unique_connection
    {
        unique_connection(unique_connection&&) = default;
        unique_connection& operator=(unique_connection&&) = default;

        unique_connection() = delete;
        unique_connection(unique_connection&) = delete;
        unique_connection& operator=(unique_connection&) = delete;

        unique_connection(connection con)
            : con_inner_(std::move(con))
        {
        }

        ~unique_connection()
        {
            con_inner_.disconnect();
        }

    private:
        connection con_inner_;
    };

    
    signal()
        : callbacks_ptr_(std::make_shared<callback_container_t>())
    {
    }

    signal(signal &&) = default;
    signal& operator=(signal &&) = default;

    signal(signal &) = delete;
    signal& operator=(signal &) = delete;


    connection connect(callback_t callback)
    {
        callbacks_ptr_->push_back(std::move(callback)); 
        return connection(callbacks_ptr_, callbacks_ptr_->size() - 1);
    }


    void operator()(Args... args)
    {
        for (auto const& callback: *callbacks_ptr_)
        {
            if (callback)
            {
                callback(args...);
            }
        }
    }

private:
    std::shared_ptr<callback_container_t> callbacks_ptr_;
};


} // namespace utils
