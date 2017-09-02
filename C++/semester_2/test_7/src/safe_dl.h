#pragma once

#include <iostream>

#include <ciso646>
#include <tuple>
#include <typeindex>
#include <string>
#include <vector>

#include "dlfcn.h"


namespace safe_dl
{

namespace detail
{
    using func_container_t = std::tuple<std::string, void*, 
          std::type_index>;
    static std::vector<detail::func_container_t> all_functions;
} // namespace detail

extern "C"
{
    void* pointer_to_all_functions = &detail::all_functions;
}


struct registrator
{
    template<class R, class... Args>
    registrator(std::string name, R (*func)(Args...))
    {
        detail::all_functions.emplace_back(std::move(name),
                reinterpret_cast<void*>(func),
                typeid(R(Args...)));
    }
};

template<class T>
struct td;

template<class Prototype>
__attribute__((used))
Prototype* 
load_function(std::string const& library,
        std::string const& function)
{
    // Shared libraries are loaded once by definition.
    void* handle_to_lib = dlopen(library.c_str(), RTLD_NOW);
    std::cout << "begin " << handle_to_lib << std::endl;
    if (handle_to_lib == NULL)
    {
        return nullptr;
    }

    std::cout << "loaded lib " << library << std::endl;
    void* func_pointers = dlsym(handle_to_lib, "pointer_to_all_functions");
    if (func_pointers == NULL)
    {
        return nullptr;
    }
    std::cout << "HERE" << std::endl;
    decltype(detail::all_functions)* funcs = 
        *reinterpret_cast<decltype(detail::all_functions)**>(func_pointers);
    std::cout << "loaded func pointers " << func_pointers << std::endl;
    std::cout << "size: " << funcs->size() << std::endl;

    std::type_index desired_type(typeid(Prototype));
    for (const auto &func: *funcs)
    {
        std::string const& name = std::get<0>(func);
        void* pointer = std::get<1>(func);
        std::type_index const& type = std::get<2>(func);
        if (name == function and type == desired_type)
        {
            return reinterpret_cast<Prototype*>(pointer);
        }
    }

    return nullptr;
}


std::vector<std::pair<std::string, void*>> get_functions()
{
    std::vector<std::pair<std::string, void*>> resulting_funcs;
    resulting_funcs.reserve(detail::all_functions.size());
    for (const auto &u: detail::all_functions)
    {
        resulting_funcs.emplace_back(std::get<0>(u), std::get<1>(u));
    }

    return resulting_funcs;
}

} // namespace safe_dl
