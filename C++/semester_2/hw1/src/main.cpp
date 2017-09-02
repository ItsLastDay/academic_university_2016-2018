#include <cassert>
#include <type_traits>
#include <iterator>
#include <iostream>
#include <vector>
#include <functional>

#include "graph.h"
#include "filtered_graph.h"
#include "shortest_path.h"

template<class T>
using is_const_ref = std::is_const<std::remove_reference_t<T>>;

template<class T>
void avoid_unused_typedef()
{}

template<class Iterator>
void check_iterator_concept(Iterator it, bool is_const)
{
    static_assert(std::is_same<std::forward_iterator_tag, typename std::iterator_traits<Iterator>::iterator_category>::value, "");
    assert(is_const_ref<decltype((*it))>::value == is_const);
    
    using a = typename std::iterator_traits<Iterator>::value_type;
    using b = typename std::iterator_traits<Iterator>::reference;
    using c = typename std::iterator_traits<Iterator>::pointer;
    using d = typename std::iterator_traits<Iterator>::difference_type;
    avoid_unused_typedef<a>();
    avoid_unused_typedef<b>();
    avoid_unused_typedef<c>();
    avoid_unused_typedef<d>();

    Iterator tmp;
    tmp = it;
    assert(tmp == it);
    assert(tmp == it++);

    Iterator tmp2;
    using std::swap;
    swap(tmp2, it);
    assert(tmp != it);
    swap(tmp2, it);

    assert(++tmp == it);
    assert(++tmp != it);


    (void)*it;
    (void)it.operator->();
}

template<class Iterator, class ConstIterator>
void check_iterator_conversion()
{
    Iterator it;
    ConstIterator cit = it;
    (void)cit;
}

using simple_graph_t    = au::graph<int, int>;

simple_graph_t make_simple_graph()
{
    au::graph<int, int> g;
    g.add_vertex(1);
    g.add_vertex(2);
    g.add_vertex(3);
    g.add_vertex(4);

    auto v1 = g.find_vertex(1);
    auto v2 = g.find_vertex(2);
    auto v3 = g.find_vertex(3);
    auto v4 = g.find_vertex(4);

    g.add_edge(v1, v2, 1);
    g.add_edge(v1, v3, 2);
    g.add_edge(v2, v3, 3);
    g.add_edge(v3, v1, 1);

    g.add_edge(v4, v1, 2);
    g.add_edge(v4, v2, 2);
    g.add_edge(v4, v3, 10);

    return g;
}

void check_graph_concept()
{
    auto g = make_simple_graph();
    auto const &cg = g;

    check_iterator_concept(g.vertex_begin(), true);
    check_iterator_concept(cg.vertex_begin(), true);
    check_iterator_concept(g.edge_begin(g.find_vertex(1)), false);
    check_iterator_concept(cg.edge_begin(cg.find_vertex(1)), true);

    check_iterator_conversion<decltype(g)::vertex_iterator, decltype(g)::vertex_const_iterator>();
    check_iterator_conversion<decltype(g)::edge_iterator, decltype(g)::edge_const_iterator>();

    static_assert(std::is_same<decltype(g)::vertex_data, int>::value, "");
    static_assert(std::is_same<decltype(g)::edge_data, int>::value, "");

    decltype(g)::vertex_iterator       v1 = g.edge_begin(g.find_vertex(1)).from();
    decltype(g)::vertex_const_iterator cv1 = cg.edge_begin(cg.find_vertex(1)).from();

    decltype(g)::vertex_iterator       v2 = g.edge_begin(g.find_vertex(1)).to();
    decltype(g)::vertex_const_iterator cv2 = cg.edge_begin(cg.find_vertex(1)).to();

    assert(*v1 == 1  && *cv1 == 1);
    assert(*v2 == 2  || *v2 == 3);
    assert(*cv2 == 2 || *cv2 == 3);
}

void check_removing()
{
    auto g = make_simple_graph();

    auto v1 = g.find_vertex(1);
    auto v2 = g.find_vertex(2);
    auto v3 = g.find_vertex(3);

    g.remove_edge(g.find_edge(v2, v3));
    assert(g.edge_begin(v2) == g.edge_end(v2));

    g.remove_edge(g.find_edge(v1, v2));
    assert(g.edge_begin(v1) != g.edge_end(v1));

    g.remove_vertex(v3);
    v1 = g.find_vertex(1);
    assert(g.edge_begin(v1) == g.edge_end(v1));
}

void check_tests_trac()
{
    auto g = make_simple_graph(); // функция из тестов
    g.remove_vertex(g.find_vertex(3));
    g.remove_vertex(g.find_vertex(2));
}

void check_edge_iterator()
{
    const int max_id = 10000;
    
    auto g = make_simple_graph();
    for (int i = 4; i < max_id; ++i)
        g.add_vertex(i);

    auto e_it = g.find_edge(g.find_vertex(1), g.find_vertex(2));

    assert(*e_it.from() == 1);
    assert(*e_it.to()   == 2);
}

using simple_filtered_t = au::filtered_graph<simple_graph_t, std::function<bool(int)>, std::function<bool(int)>>;

simple_filtered_t make_simple_filtered(simple_graph_t const &g)
{
    auto vertex_predicate = [](int vertex_data)
    {
        return vertex_data != 3;
    };

    auto edge_predicate = [](int edge_data)
    {
        return edge_data != 1;
    };

    return simple_filtered_t(g, vertex_predicate, edge_predicate);
}

void check_filtered_graph()
{
    auto g  = make_simple_graph();
    auto fg = make_simple_filtered(g);

    auto v1 = fg.find_vertex(1);
    auto v2 = fg.find_vertex(2);
    auto v3 = fg.find_vertex(3);
    auto v4 = fg.find_vertex(4);

    assert(v1 != fg.vertex_end());
    assert(v2 != fg.vertex_end());
    assert(v3 == fg.vertex_end());
    assert(v4 != fg.vertex_end());

    check_iterator_concept(fg.vertex_begin(), true);
    check_iterator_concept(fg.edge_begin(v4), true);

    using std::distance;
    assert(distance(fg.edge_begin(v1), fg.edge_end(v1)) == 0);
    assert(distance(fg.edge_begin(v2), fg.edge_end(v2)) == 0);
    assert(distance(fg.edge_begin(v4), fg.edge_end(v4)) == 2);

    assert(fg.find_edge(v1, v2) == fg.edge_end(v1));
    assert(*fg.find_edge(v4, v2) == 2);
}

template<class Graph>
std::vector<int> collect_vertex_path(Graph const &g, int vertex_from, int vertex_to)
{
    auto it_from = g.find_vertex(vertex_from);
    auto it_to   = g.find_vertex(vertex_to);

    std::vector<int> res;
    auto edge_visitor = [&res](typename Graph::edge_const_iterator e_it)
    {
        res.push_back(*e_it.from());
    };

    auto len_functor = [](int value)
    {
        return static_cast<double>(value);
    };

    bool path_found = au::find_shortest_path(g, it_from, it_to, len_functor, edge_visitor);

    assert(path_found != res.empty() || it_from == it_to);
    if (path_found)
        res.push_back(*it_to);

    return res;
}

void check_shortest_path()
{
    auto g  = make_simple_graph();
    auto fg = make_simple_filtered(g);

    assert((collect_vertex_path(g, 1, 2) == std::vector<int>{1, 2}));
    assert((collect_vertex_path(g, 1, 3) == std::vector<int>{1, 3}));
    assert((collect_vertex_path(g, 1, 4) == std::vector<int>{}));
    assert((collect_vertex_path(g, 1, 1) == std::vector<int>{1}));

    assert((collect_vertex_path(g, 4, 1) == std::vector<int>{4, 1}));
    assert((collect_vertex_path(g, 4, 3) == std::vector<int>{4, 1, 3}));
    assert((collect_vertex_path(g, 4, 4) == std::vector<int>{4}));

    assert((collect_vertex_path(fg, 4, 1) == std::vector<int>{4, 1}));
    assert((collect_vertex_path(fg, 1, 2) == std::vector<int>{}));
    assert((collect_vertex_path(fg, 4, 4) == std::vector<int>{4}));
}

int main()
{
    check_graph_concept();
    check_removing();
    check_edge_iterator();
    check_tests_trac();
    check_filtered_graph();
    check_shortest_path();
    std::cout << "everything all right, we did it" << std::endl;
    return 0;
}
