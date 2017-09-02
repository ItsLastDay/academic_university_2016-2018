#pragma once

#include <iterator>
#include <unordered_map>
#include <vector>
#include <utility>
#include <list>


namespace au
{

template<class vertex_data, class edge_data>
struct graph;

template<class graph, class vertex_filter, class edge_filter>
struct filtered_graph;


namespace detail
{

template<class iterator,
         bool is_filtered,
         bool is_edge,
         bool is_const>
struct iterator_deref_impl
{};    


template<>
template<class iterator>
struct iterator_deref_impl<iterator, false, false, true>
{
    auto& operator*() const
    {
        return static_cast<iterator const*>(this)->underlying_iterator_->first;
    }
};

template<class iterator>
struct iterator_deref_impl<iterator, false, true, false>
{
    auto& operator*()
    {
        return static_cast<iterator*>(this)->underlying_iterator_->second;
    }
};

template<class iterator>
struct iterator_deref_impl<iterator, false, true, true>
{
    auto& operator*() const
    {
        return static_cast<iterator const*>(this)->underlying_iterator_->second;
    }
};

template<class iterator, bool is_edge>
struct iterator_deref_impl<iterator, true, is_edge, true>
{
    auto& operator*() const
    {
        return *static_cast<iterator const*>(this)->underlying_iterator_;
    }
};


template<class iterator, 
         bool is_filtered, 
         bool is_edge>
struct iterator_inc_impl
{
};


template<>
template<class iterator, bool is_edge>
struct iterator_inc_impl<iterator, false, is_edge>
{
    iterator& operator++()
    {
        ++static_cast<iterator*>(this)->underlying_iterator_;
        return *static_cast<iterator*>(this);
    }
};


template<>
template<class iterator>
struct iterator_inc_impl<iterator, true, true>
{
    iterator& operator++()
    {
        iterator& ref = *static_cast<iterator*>(this);
        auto& under_it = ref.underlying_iterator_;
        ++under_it;
        while (under_it != ref.underlying_container_->edge_end(under_it.from()))
        {
            if ((*ref.vertex_filter_)(*under_it.from()) 
                    and (*ref.vertex_filter_)(*under_it.to())
                    and (*ref.edge_filter_)(*under_it))
            {
                break;
            }
            ++under_it;
        }
        
        return ref;
    }
};


template<>
template<class iterator>
struct iterator_inc_impl<iterator, true, false>
{
    iterator& operator++()
    {
        iterator& ref = *static_cast<iterator*>(this);
        auto& under_it = ref.underlying_iterator_;
        ++under_it;
        
        while (under_it != ref.underlying_container_->vertex_end())
        {
            if ((*ref.vertex_filter_)(*under_it))
            {
                break;
            }
            ++under_it;
        }
        return ref;
    }
};


template<class iterator,
         bool is_filtered,
         bool is_edge>
struct iterator_from_to_impl
{};


template<>
template<class iterator>
struct iterator_from_to_impl<iterator, true, true>
{
    auto from() const
    {
        iterator const& ref = *static_cast<iterator const*>(this);
        return ref.main_container_->find_vertex(*ref.underlying_iterator_.from());
    }

    auto to() const
    {
        iterator const& ref = *static_cast<iterator const*>(this);
        return ref.main_container_->find_vertex(*ref.underlying_iterator_.to());
    }
};

template<>
template<class iterator>
struct iterator_from_to_impl<iterator, false, true>
{
    auto from() const
    {
        iterator const& ref = *static_cast<iterator const*>(this);
        return ref.main_container_->find_vertex(*ref.vertex_data_);
    }

    auto to() const
    {
        iterator const& ref = *static_cast<iterator const*>(this);
        return ref.main_container_->find_vertex(ref.underlying_iterator_->first);
    }
};


template<class underlying_iterator,
         class underlying_container,
         class main_container,
         class returned_value,
         class vertex_data,
         class edge_data,
         class vertex_filter,
         class edge_filter,
         bool is_filtered,
         bool is_edge,
         bool is_const>
struct general_iterator
    : iterator_deref_impl< 
            general_iterator<underlying_iterator, underlying_container, main_container, returned_value, vertex_data, edge_data, vertex_filter, edge_filter, is_filtered, is_edge, is_const>,
            is_filtered, is_edge, is_const> 
    , iterator_from_to_impl<
            general_iterator<underlying_iterator, underlying_container, main_container, returned_value, vertex_data, edge_data, vertex_filter, edge_filter, is_filtered, is_edge, is_const>,
            is_filtered, is_edge> 
    , iterator_inc_impl<
            general_iterator<underlying_iterator, underlying_container, main_container, returned_value, vertex_data, edge_data, vertex_filter, edge_filter, is_filtered, is_edge, is_const>,
            is_filtered, is_edge> 
{
    using value_type = returned_value;
    using difference_type = size_t;
    using reference = value_type&;
    using pointer = value_type*;

    using iterator_category = std::forward_iterator_tag;

    using iterator_inc_impl<
            general_iterator<underlying_iterator, underlying_container, main_container, returned_value, vertex_data, edge_data, vertex_filter, edge_filter, is_filtered, is_edge, is_const>,
            is_filtered, is_edge>::operator++;

    general_iterator(underlying_iterator it,
                  underlying_container const* u_cont,
                  main_container const* m_cont,
                  vertex_data const* v_data,
                  vertex_filter const* v_filter,
                  edge_filter const* e_filter)
        : underlying_iterator_(it)
        , underlying_container_(u_cont)
        , main_container_(m_cont)
        , vertex_data_(v_data)
        , vertex_filter_(v_filter)
        , edge_filter_(e_filter)
    {
    }

    template<class T, bool is_const_here=is_const, typename std::enable_if<is_const_here, int>::type = 0> 
    general_iterator(general_iterator<T,
                                underlying_container,
                                main_container,
                                typename std::remove_const_t<returned_value>,
                                vertex_data,
                                edge_data,
                                vertex_filter,
                                edge_filter,
                                is_filtered,
                                is_edge,
                                false> const& other)
        : underlying_iterator_(other.underlying_iterator_)
        , underlying_container_(other.underlying_container_)
        , main_container_(other.main_container_)
        , vertex_data_(other.vertex_data_)
        , vertex_filter_(other.vertex_filter_)
        , edge_filter_(other.edge_filter_)
    {
    }

    general_iterator()
        : underlying_container_(nullptr)
    {
    }

    general_iterator(general_iterator const&) = default;
    general_iterator& operator=(general_iterator const&) = default;

    ~general_iterator() = default;

    void swap(general_iterator& other)
    {
        std::swap(underlying_iterator_, other.underlying_iterator_);
        std::swap(underlying_container_, other.underlying_container_);
        std::swap(main_container_, other.main_container_);
        std::swap(vertex_data_, other.vertex_data_);
        std::swap(vertex_filter_, other.vertex_filter_);
        std::swap(edge_filter_, other.edge_filter_);
    }

    pointer operator->()
    {
        return &(**this);
    }

    general_iterator operator++(int)
    {
        general_iterator result = *this;
        ++(*this);
        return result;
    }

    bool operator==(general_iterator const& other) const
    {
        if (other.underlying_container_ == nullptr or underlying_container_ == nullptr)
        {
            return other.underlying_container_ == nullptr and underlying_container_ == nullptr;
        }

        return underlying_iterator_ == other.underlying_iterator_;
    }

    bool operator!=(general_iterator const& other) const
    {
        return !(*this == other);
    }

private:
    underlying_iterator underlying_iterator_;

    underlying_container const* underlying_container_;
    main_container const* main_container_;

    vertex_data const* vertex_data_;
    vertex_filter const* vertex_filter_;
    edge_filter const* edge_filter_;

    friend struct filtered_graph<graph<vertex_data, edge_data>, vertex_filter, edge_filter>;
    friend struct graph<vertex_data, edge_data>;
    template<class, bool, bool> friend struct iterator_from_to_impl;
    template<class, bool, bool> friend struct iterator_inc_impl;
    template<class, bool, bool, bool> friend struct iterator_deref_impl;

    // Slightly more permissive than I want.
    template<class, class, class, class, class, class, class, class, bool, bool, bool>
    friend struct general_iterator;
};

} // namespace detail

template<class vertex_data_, class edge_data_>
struct graph
{
public:
    using vertex_data = vertex_data_;
    using edge_data = edge_data_;

private: 
    using container_t = std::unordered_map<vertex_data, 
          std::list<std::pair<vertex_data, edge_data>>>;

public:
    using vertex_iterator = detail::general_iterator<typename container_t::const_iterator,
                                                   container_t,
                                                   void,
                                                   vertex_data const,
                                                   vertex_data,
                                                   edge_data,
                                                   void,
                                                   void,
                                                   false,
                                                   false,
                                                   true>;

    using edge_iterator = detail::general_iterator<typename container_t::mapped_type::iterator,
                                                 container_t,
                                                 graph<vertex_data, edge_data>,
                                                 edge_data,
                                                 vertex_data,
                                                 edge_data,
                                                 void,
                                                 void,
                                                 false,
                                                 true,
                                                 false>;

    using vertex_const_iterator = vertex_iterator; 
    using edge_const_iterator = detail::general_iterator<typename container_t::mapped_type::const_iterator,
                                                 container_t,
                                                 graph<vertex_data, edge_data>,
                                                 edge_data const,
                                                 vertex_data,
                                                 edge_data,
                                                 void,
                                                 void,
                                                 false,
                                                 true,
                                                 true>;

    graph() = default;
    graph(graph const&) = default;
    graph(graph&&) = default;

    graph& operator=(graph const&) = default;
    graph& operator=(graph &&) = default;

    vertex_iterator add_vertex(vertex_data data)
    {
        auto vert_it = vertex_to_incident_.find(data);
        if (vert_it == vertex_to_incident_.end())
        {
            typename container_t::mapped_type empty_lst;
            vertex_to_incident_[data] = empty_lst;
        }

        return make_vertex_iterator(vertex_to_incident_.find(data));
    }

    edge_iterator add_edge(vertex_iterator const& from,
            vertex_iterator const& to, edge_data data)
    {
        auto& lst = vertex_to_incident_.find((*from))->second;
        vertex_data const to_data = *to;
        lst.emplace_front(to_data, data);
        return make_edge_iterator(lst.begin(), &(*from));
    }

    void remove_vertex(vertex_iterator del_it)
    {
        vertex_data deleted_data = *del_it;
        for (auto& vert_lst_pair: vertex_to_incident_)
        {
            vert_lst_pair.second.remove_if([&deleted_data](auto x) 
                    { return x.first == deleted_data; });
        }
        vertex_to_incident_.erase(del_it.underlying_iterator_);
    }

    void remove_edge(edge_iterator it)
    {
        auto it_from = it.from();
        auto& lst = vertex_to_incident_.find(*it_from)->second;
        lst.erase(it.underlying_iterator_);
    }

    vertex_iterator find_vertex(vertex_data const& data)
    {
        return make_vertex_iterator(vertex_to_incident_.find(data));
    }

    vertex_const_iterator find_vertex(vertex_data const& data) const
    {
        return const_cast<graph*>(this)->find_vertex(data);
    }

    edge_iterator find_edge(vertex_iterator from, vertex_iterator to)
    {
        vertex_data const to_data = *to;
        auto& lst = vertex_to_incident_.find(*from)->second;
        for (auto lst_elem_it = lst.begin(); lst_elem_it != lst.end(); ++lst_elem_it)
        {
            if (lst_elem_it->first == to_data)
            {
                return make_edge_iterator(lst_elem_it, &(*from));
            }
        }

        return edge_iterator();
    }

    edge_const_iterator find_edge(vertex_iterator from, vertex_iterator to) const
    {
        // Reuse the above implementation: it really does not modify anything.
        return const_cast<graph*>(this)->find_edge(from, to);
    }

    vertex_iterator vertex_begin()
    {
        return make_vertex_iterator(vertex_to_incident_.begin());
    }

    vertex_const_iterator vertex_begin() const
    {
        return const_cast<graph*>(this)->vertex_begin();
    }

    vertex_iterator vertex_end()
    {
        return make_vertex_iterator(vertex_to_incident_.end());
    }

    vertex_const_iterator vertex_end() const
    {
        return const_cast<graph*>(this)->vertex_end();
    }

    edge_iterator edge_begin(vertex_iterator from)
    {
        // Assuming that vertex is inside graph.
        auto& cont = vertex_to_incident_.find(*from)->second;
        return make_edge_iterator(cont.begin(), &(*from));
    }

    edge_const_iterator edge_begin(vertex_iterator from) const
    {
        return const_cast<graph*>(this)->edge_begin(from);
    }

    edge_iterator edge_end(vertex_iterator from)
    {
        auto& cont = vertex_to_incident_.find(*from)->second;
        return make_edge_iterator(cont.end(), &(*from));
    }

    edge_const_iterator edge_end(vertex_iterator from) const
    {
        return const_cast<graph*>(this)->edge_end(from);
    }

private:
     edge_iterator make_edge_iterator(typename container_t::mapped_type::iterator it,
                                      vertex_data const* from_data)
     {
         return edge_iterator(it, &vertex_to_incident_, this, 
                 from_data, nullptr, nullptr);
     }

     edge_const_iterator make_const_edge_iterator(typename container_t::mapped_type::const_iterator it,
                                      vertex_data const* from_data) const
     {
         return edge_const_iterator(it, &vertex_to_incident_, this, 
                 from_data, nullptr, nullptr);
     }
    
     vertex_iterator make_vertex_iterator(typename container_t::const_iterator it) const
     {
         return vertex_iterator(it, &vertex_to_incident_, this, 
                 nullptr, nullptr, nullptr);
     }


     container_t vertex_to_incident_;

};


} // namespace au
