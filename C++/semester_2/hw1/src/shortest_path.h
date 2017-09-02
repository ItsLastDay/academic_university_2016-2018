#pragma once

#include <cmath>
#include <limits>
#include <type_traits>
#include <set>
#include <functional>
#include <vector>
#include <utility>
#include <unordered_map>


namespace au
{

namespace detail
{

struct vertex_iterator_hasher
{
    template<class T>
    size_t operator()(T const& it) const noexcept 
    {
        return std::hash<std::decay_t<decltype(*it)>>()(*it);
    }
};

struct first_pair_comparer
{
    template<class T>
    bool operator()(std::pair<double, T> const& lhs, std::pair<double, T> const& rhs) 
    const noexcept
    {
        if (fabs(lhs.first - rhs.first) < 1e-15)
        {
            return vertex_iterator_hasher()(lhs.second) < vertex_iterator_hasher()(rhs.second);
        }

        return lhs.first < rhs.first;
    }
};

} // namespace detail

template<class graph, class edge_len_, class path_visitor>
bool find_shortest_path(
        graph const& gr,
        typename graph::vertex_const_iterator from,
        typename graph::vertex_const_iterator to,
        edge_len_&& len_functor,
        path_visitor&& visitor)
{
    using hasher = detail::vertex_iterator_hasher;
    std::set<std::pair<double, decltype(from)>, detail::first_pair_comparer> queue;
    std::unordered_map<decltype(from), double, hasher> current_distance;
    std::unordered_map<decltype(from), typename graph::edge_const_iterator, hasher> prev_edge;

    queue.insert(std::make_pair(0, from));
    current_distance[from] = 0;

    while (not queue.empty())
    {
        auto cur_pair = *queue.begin();
        queue.erase(queue.begin());
        double cur_dist = cur_pair.first;
        decltype(from) cur_vertex = cur_pair.second;

        if (cur_vertex == to)
        {
            break;
        }

        for (auto edge_it = gr.edge_begin(cur_vertex);
                edge_it != gr.edge_end(cur_vertex);
                ++edge_it)
        {
            decltype(from) vert_to = edge_it.to();
            double edge_len = len_functor(*edge_it);

            double current_dist_to_vert = std::numeric_limits<double>::max();
            if (current_distance.count(vert_to))
            {
                current_dist_to_vert = current_distance[vert_to];
            }

            if (edge_len + cur_dist < current_dist_to_vert)
            {
                queue.erase(std::make_pair(current_dist_to_vert, vert_to));
                current_distance[vert_to] = edge_len + cur_dist;
                queue.insert(std::make_pair(edge_len + cur_dist, vert_to));
                prev_edge[vert_to] = edge_it;
            }
        }
    }


    if (current_distance.count(to) == 0)
    {
        return false;
    }

    std::vector<typename graph::edge_const_iterator> visited_edges;
    
    while (to != from)
    {
        auto edge = prev_edge[to];
        visited_edges.push_back(edge);
        to = edge.from();
    }

    for (auto it = visited_edges.rbegin(); it != visited_edges.rend(); ++it)
    {
        visitor(*it);
    }

    return true;
}


} // namespace au
