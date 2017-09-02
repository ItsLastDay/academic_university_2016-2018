#pragma once

#include <cstdio>

namespace my_stl_funcs
{

template<class T>
void fill(T* begin, T* end, T const& value)
{
    T* it = begin;
    while (it != end)
    {
        *it = value;
        ++it;
    }
}


template<class T, class Gen>
void generate(T* begin, T* end, Gen generator)
{
    T* it = begin;
    while (it != end)
    {
        *it = generator();
        ++it;
    }
}


template<class T>
void copy(T* src_begin, T* src_end, T* dest_it)
{
    T* src_it = src_begin;
    while (src_it != src_end)
    {
        *dest_it = *src_it;
        ++dest_it;
        ++src_it;
    }
}


template<class T, class Functor>
void for_each(T* begin, T* end, Functor functor)
{
    T* it = begin;
    while (it != end)
    {
        functor(*it);
        ++it;
    }
}



} // namespace my_stl_funcs
