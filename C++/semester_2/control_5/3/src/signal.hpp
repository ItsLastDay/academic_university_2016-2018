#pragma once

#include <functional>
#include <memory>
#include <list>
#include <utility>



namespace utils
{

template<class>
struct signal;


template<class R, class... Args>
struct signal<R(Args...)>
{
private:
    using callback_t = std::function<R(Args...)>;
    using clb_wrapper_t = std::pair<callback_t, bool>;
    using clb_wrapper_pnt_t = std::shared_ptr<clb_wrapper_t>;
    using callback_container_t = 
        std::list<clb_wrapper_pnt_t>;

public:
    struct connection
    {
        connection(connection &&) = default;
        connection & operator=(connection&& ) = default;

        connection() = delete;
        connection(connection &) = delete;
        connection& operator=(connection &) = delete;

        connection(clb_wrapper_pnt_t clb)
            : weak_pnt_wrapper_(clb)
        {
        }

        void disconnect()
        {
            if (weak_pnt_wrapper_.expired())
            {
                return;
            }

            weak_pnt_wrapper_.lock()->second = false;
        }

    private:
        std::weak_ptr<clb_wrapper_t> weak_pnt_wrapper_;
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

    
    signal() = default;
    signal(signal &&) = default;
    signal& operator=(signal &&) = default;

    signal(signal &) = delete;
    signal& operator=(signal &) = delete;


    connection connect(callback_t callback)
    {
        auto desc_pair_ptr = 
            std::make_shared<clb_wrapper_t>(std::move(callback), true);
        deferred_added_callbacks_.push_back(desc_pair_ptr);
        return connection(desc_pair_ptr);
    }


    void operator()(Args... args)
    {
        add_deferred();

        for (auto const& pnt: main_callbacks_)
        {
            clb_wrapper_t pp = *pnt;
            if (pp.second == false)
            {
                continue;
            }

            pp.first(args...);
        }

        for (auto it = main_callbacks_.begin(); it !=
                main_callbacks_.end(); )
        {
            if ((*it)->second == false)
            {
                it = main_callbacks_.erase(it);
            } 
            else
            {
                ++it;
            }
        }
    }

private:
    void add_deferred()
    {
        main_callbacks_.insert(main_callbacks_.end(),
                deferred_added_callbacks_.begin(),
                deferred_added_callbacks_.end());
        deferred_added_callbacks_.clear();
    }

    callback_container_t main_callbacks_;
    callback_container_t deferred_added_callbacks_;
};


} // namespace utils
