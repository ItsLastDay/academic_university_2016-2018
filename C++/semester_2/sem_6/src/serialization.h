#pragma once

#include <type_traits>
#include <iostream>
#include <utility>


namespace serialization
{

template<class T>
class is_container
{
    template<class U>
    static std::true_type check(typename U::iterator*);

    template<class U>
    static std::false_type check(...);

public:
    static const bool value = decltype(check<T>(nullptr))();
};


template<class T>
using is_container_t = typename std::enable_if<is_container<T>::value>::type;


template<class T>
using is_pod_t = typename std::enable_if<std::is_pod<T>::value>::type;


template<class F, class S>
void 
serialize(std::ostream& out, std::pair<F, S> const& p);


template<class F, class S>
void 
deserialize(std::istream& in, std::pair<F, S> & p);


template<class T>
is_pod_t<T>
serialize(std::ostream& out, T const& obj) 
{
    out.write((char*)&obj, sizeof(obj));
}


template<class Container>
is_container_t<Container>
serialize(std::ostream& out, Container const& cont)
{
    typename Container::size_type sz = cont.size();
    out.write((char*)&sz, sizeof(sz));

    for (auto const& val: cont)
    {
        serialize(out, val);
    }
}


template<class F, class S>
void 
serialize(std::ostream& out, std::pair<F, S> const& p)
{
    serialize(out, p.first);
    serialize(out, p.second);
}


template<class T>
is_pod_t<T>
deserialize(std::istream& in, T& obj)
{
    in.read((char*)&obj, sizeof(obj));
}


template<class Container>
is_container_t<Container>
deserialize(std::istream& in, Container& cont)
{
    typename Container::size_type sz;
    in.read((char*)&sz, sizeof(sz));

    for (typename Container::size_type idx = 0;
            idx < sz; idx++)
    {
        typename Container::value_type val; // won't work if there is no default constructor.
        deserialize(in, val);
        cont.insert(cont.cend(), val);
    }
}


template<class F, class S>
void 
deserialize(std::istream& in, std::pair<F, S> & p)
{
    // For the purposes of std::map serialization, only the first 
    // argument needs to be const-uncasted.
    deserialize(in, *const_cast<std::remove_const_t<F>*>(&p.first));
    deserialize(in, p.second);
}


} // namespace serialization
