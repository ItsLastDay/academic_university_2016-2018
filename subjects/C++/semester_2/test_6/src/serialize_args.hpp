#pragma once

#include <vector>
#include <string>
#include <type_traits>
#include <tuple>
#include <utility>




std::vector<char> store_args() 
{
    return std::vector<char>();
}

template<class... Args>
std::vector<char>
store_args(std::string const& s, Args... args);

template<class Arith, class... Args>
std::vector<char>
store_args(Arith const& val, Args... args)
{
    std::vector<char> stored_args;
    size_t val_size = sizeof(val);
    stored_args.resize(val_size);
    for (size_t i = 0; i < val_size; i++)
    {
        stored_args[i] = reinterpret_cast<const char*>(&val)[i];
    }

    auto recursive_stored_args = store_args(args...);
    stored_args.insert(stored_args.end(), recursive_stored_args.begin(),
            recursive_stored_args.end());

    return stored_args;
}

template<class... Args>
std::vector<char> 
store_args(std::string const &s, Args... args)
{
    std::vector<char> stored_args = store_args(s.size());
    for (size_t i = 0; i < s.size(); i++)
    {
        stored_args.push_back(s[i]);
    }

    auto recursive_stored_args = store_args(args...);
    stored_args.insert(stored_args.end(), recursive_stored_args.begin(),
            recursive_stored_args.end());

    return stored_args;
}


template<class Arith>
void
restore_one_arg(Arith& val, std::vector<char>& stored_args)
{
    size_t val_size = sizeof(val);
    for (size_t i = 0; i < val_size; i++)
    {
        reinterpret_cast<char*>(&val)[i] = stored_args[i];
    }

    stored_args.erase(stored_args.begin(), stored_args.begin() + val_size);
}


void
restore_one_arg(std::string& s, std::vector<char>& stored_args)
{
    typename std::string::size_type s_size;
    restore_one_arg(s_size, stored_args);
    
    s = "";
    for (size_t i = 0; i < s_size; i++)
    {
        s += stored_args[i];
    }

    stored_args.erase(stored_args.begin(), stored_args.begin() + s.size());
}


template<class... Args, size_t... I>
void
iterate_restore(std::tuple<Args...>& restored_tuple, std::vector<char> & stored_args,
        std::index_sequence<I...>)
{
    int dummy[] = { (restore_one_arg(std::get<I>(restored_tuple), stored_args), 0)... };
}


template<class... Args>
std::tuple<Args...>
restore_args(std::vector<char> const& stored_args)
{
    std::tuple<Args...> restored_tuple;
    auto copy_stored_args = stored_args;
    iterate_restore(restored_tuple, copy_stored_args, std::make_index_sequence<sizeof...(Args)>());
    return restored_tuple;
}


template<class Real, class T>
void save_one_arg(std::vector<char>& vec, T&& arg)
{
    Real real_arg(std::forward<T>(arg));
    auto res = store_args(real_arg);
    vec.insert(vec.end(), res.begin(), res.end());
}

template<class...FuncArgs, class...InArgs, size_t... I>
std::vector<char> save_args_inner(std::tuple<FuncArgs...>& fnc, std::tuple<InArgs...> && args,
        std::index_sequence<I...>)
{
    std::vector<char> result;
    int dummy[] = {
        (save_one_arg<typename std::remove_reference<decltype(std::get<I>(fnc))>::type,
         decltype(std::get<I>(args))>(result, std::get<I>(args)), 0)...
    };
    return result;
}


template<class R, class... FuncArgs, class... InArgs>
std::vector<char> save_args(R(*)(FuncArgs...), InArgs&&... args)
{
    std::tuple<FuncArgs...> dm;
    return save_args_inner(dm, std::make_tuple<InArgs...>(std::forward<InArgs>(args)...),
            std::make_index_sequence<sizeof...(InArgs)>());
}


