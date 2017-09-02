#include <bits/stdc++.h>
using namespace std;


template<class... Args>
void print_tuple_rec(tuple<Args...> const& x)
{
    cout << "end" << endl;
};


// Inspiration from http://stackoverflow.com/a/10627597/5338270
template<size_t N, class Tuple>
struct tuple_tail
{
    template<class... Args>
    static auto create(Tuple const& x, Args&&... args)
    {
        return tuple_tail<N - 1, Tuple>::create(x, get<N>(x), args...);
    }
};

template<class Tuple>
struct tuple_tail<0, Tuple>
{
    template<class... Args>
    static tuple<Args...> create(Tuple const& x, Args&&... args)
    {
        return tuple<Args...>(args...);
    }
};


template<class Head, class... Tail>
void print_tuple_rec(tuple<Head, Tail...> const& x)
{
    cout << get<0>(x) << endl;
    print_tuple_rec(tuple_tail<sizeof...(Tail), decltype(x)>::create(x));
}

void f(int)
{
    cout << "0" << endl;
}

template<class Tup, size_t... I>
void print_tuple_inner(Tup const& x, index_sequence<I...>)
{
    // looked at cppreference `index_sequence` examples.
    int v[] = { (cout << get<I>(x) << " ", 0)... };
}

template<class... Args>
void print_tuple_nonrec(tuple<Args...> const& x)
{
    print_tuple_inner(x, make_index_sequence<sizeof...(Args)>{});
    cout << endl;
}

int main()
{
    print_tuple_rec(make_tuple(1, "asd"));
    print_tuple_rec(make_tuple('t'));
    cout << endl;
    print_tuple_rec(make_tuple(1, "asd", 45.0, bitset<50>()));
    cout << endl << endl;

    print_tuple_nonrec(make_tuple(1, "asd"));
    print_tuple_nonrec(make_tuple('t'));
    print_tuple_nonrec(make_tuple(1, "asd", 45.0, bitset<50>()));

    f(5);
    return 0;
}
