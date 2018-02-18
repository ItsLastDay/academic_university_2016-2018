#pragma once

#include <iterator>
#include <unordered_map>
#include <vector>
#include <utility>
#include <list>

#include "graph.h"


namespace au
{

template<class graph, class vertex_filter, class edge_filter>
struct filtered_graph
{
    using vertex_data = typename graph::vertex_data;
    using edge_data = typename graph::edge_data;

    using vertex_iterator = detail::general_iterator<typename graph::vertex_const_iterator,
                                                      graph,
                                                      filtered_graph<graph, vertex_filter, edge_filter>,
                                                      vertex_data const,
                                                      vertex_data,
                                                      edge_data,
                                                      vertex_filter,
                                                      edge_filter,
                                                      true,
                                                      false,
                                                      true>;

    using edge_iterator =   detail::general_iterator<typename graph::edge_const_iterator,
                                                      graph,
                                                      filtered_graph<graph, vertex_filter, edge_filter>,
                                                      edge_data const,
                                                      vertex_data,
                                                      edge_data,
                                                      vertex_filter,
                                                      edge_filter,
                                                      true,
                                                      true,
                                                      true>;

    using vertex_const_iterator = vertex_iterator;
    using edge_const_iterator = edge_iterator;

    filtered_graph(graph const& graph_ref, vertex_filter&& vert_filter, edge_filter&& ed_filter)
        : v_filter_(vert_filter)
        , e_filter_(ed_filter)
        , graph_(&graph_ref)
    {
    }

    vertex_iterator find_vertex(vertex_data const& data) const
    {
        auto it_plain_graph = graph_->find_vertex(data);
        if (not v_filter_(data))
        {
            it_plain_graph = graph_->vertex_end();
        }
        return make_vertex_iterator(it_plain_graph);
    }

    edge_iterator find_edge(vertex_iterator from, vertex_iterator to) const
    {
        auto& from_underlying = from.underlying_iterator_;
        auto it_plain_graph = graph_->find_edge(from_underlying, to.underlying_iterator_);
        if (it_plain_graph != graph_->edge_end(from_underlying) and not e_filter_(*it_plain_graph))
        {
            it_plain_graph = graph_->edge_end(from_underlying);
        }
        return make_edge_iterator(it_plain_graph);
    }

    vertex_iterator vertex_begin() const
    {
        auto it_filtered = make_vertex_iterator(graph_->vertex_begin());
        if (it_filtered != vertex_end() and not v_filter_(*it_filtered))
        {
            ++it_filtered;
        }
        return it_filtered;
    }

    vertex_iterator vertex_end() const
    {
        return make_vertex_iterator(graph_->vertex_end());
    }

    edge_iterator edge_begin(vertex_iterator from) const
    {
        auto it_plain_graph = graph_->edge_begin(from.underlying_iterator_);
        for (; it_plain_graph != graph_->edge_end(from.underlying_iterator_); ++it_plain_graph)
        {
            auto it_from = it_plain_graph.from();
            auto it_to = it_plain_graph.to();
            if (v_filter_(*it_from) and v_filter_(*it_to) and
                    e_filter_(*it_plain_graph))
            {
                break;
            }
        }

        return make_edge_iterator(it_plain_graph);
    }

    edge_iterator edge_end(vertex_iterator from) const
    {
        return make_edge_iterator(graph_->edge_end(from.underlying_iterator_));
    }

private:
    edge_iterator make_edge_iterator(typename graph::edge_const_iterator graph_it) const
    {
        return edge_iterator(graph_it, graph_, this, nullptr, &v_filter_, &e_filter_);
    }

    vertex_iterator make_vertex_iterator(typename graph::vertex_const_iterator graph_it) const
    {
        return vertex_iterator(graph_it, graph_, this, nullptr, &v_filter_, &e_filter_);
    }

    vertex_filter v_filter_;
    edge_filter e_filter_;
    graph const* graph_;
};

} // namespace au
