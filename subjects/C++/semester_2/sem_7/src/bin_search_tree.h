#pragma once
#include <memory>
#include <stdexcept>
#include <cassert>
#include <string>
#include <sstream>
#include <iterator>
#include <vector>
#include <utility>


namespace details
{

template<class KEY, class VALUE>
struct node
{
    node(const KEY &key, const VALUE &val)
        : left_(nullptr)
        , right_(nullptr)
        , parent_(nullptr)
        , key_(key)
        , value_(val)
    {}

    ~node() noexcept
    {
        // Delete subtree without recursion. Two cases:
        // -`this` is a root of non-trivial tree (with more than one node);
        // -`this` is a tree of one node.
        node* cur_node = this;

        // Definitely more readable than "!left_ && !right_".
        if (not left_ and not right_)
        {
            return;
        }
        
        while (cur_node)
        {
            node* nxt_node;
            if (cur_node->left() != nullptr)
            {
                nxt_node = cur_node->left();
                cur_node->left_.release();
            }
            else if (cur_node->right() != nullptr)
            {
                nxt_node = cur_node->right();
                cur_node->right_.release();
            }
            else
            {
                nxt_node = cur_node->parent();

                // Destructor for `this` was called, so 
                // no need to `delete` it.
                // Other nodes need to be deleted: this function manages them.
                if (cur_node != this)
                {
                    delete cur_node;
                }
            }

            cur_node = nxt_node;
        }
    }

    // only move semantics are possible

    void putLeft(std::unique_ptr<node> &&nptr) noexcept
    {
        left_ = std::move(nptr);

        if (left_)
        {
            left_->parent_ = this;
        }
    }

    std::unique_ptr<node> takeLeft() noexcept
    {
        if (left_)
        {
            left_->parent_ = nullptr;
        }

        return std::move(left_);
    }

    void putRight(std::unique_ptr<node> &&nptr) noexcept
    {
        right_ = std::move(nptr);

        if (right_)
        {
            right_->parent_ = this;
        }
    }

    std::unique_ptr<node> takeRight() noexcept
    {
        if (right_)
        {
            right_->parent_ = nullptr;
        }

        return std::move(right_);
    }

    node*& parent() noexcept
    {
        return parent_;
    }

    node* left() noexcept
    {
        return left_.get();
    }

    node* right() noexcept
    {
        return right_.get();
    }

    KEY& key() noexcept
    {
        return key_;
    }

    VALUE& value() noexcept
    {
        return value_;
    }

private:
    std::unique_ptr<node> left_, right_;
    node* parent_;
    KEY key_;
    VALUE value_; 
};

} // namespace details

struct bst_key_exists_exception : std::exception 
{
    virtual const char* what()
    {
        return "Key already exists in BST.";
    }
}; 

struct bst_key_absent_exception : std::exception 
{
    virtual const char* what() 
    {
        return "Key is not present in BST.";
    }
};

struct bst_key_exists_exception_detailed : bst_key_exists_exception
{
    template<class Key>
    bst_key_exists_exception_detailed(Key const& key)
    {
        std::ostringstream out;
        out << "Key '" << key << "' is already in BST.";
        out.flush();
        msg_ = out.str();
    }

    virtual const char* what()
    {
        return msg_.c_str();
    }

private:
    std::string msg_;
};


// KEY should have operator <, <<, () constructor
template<class KEY, class VALUE>
struct bin_search_tree
{
private:
    using node_t = details::node<KEY, VALUE>;

    // Possible key position can be described by three things:
    //   1) Pointer to node that contains this key (nullptr if there is 
    //      no such node);
    //   2) Pointer to parent node (nullptr if #1 was pointer to root);
    //   3) Whether #1 is the left parent of #2 (it can either be left or right,
    //      so boolean is enough).
    //
    // If the tree is empty, for any key the position will be <nullptr, nullptr, false>
    using node_position_desc = std::tuple<node_t*, node_t*, bool>;

public:
    bin_search_tree()
        : root_(nullptr)
    {}

    // throws bst_key_exists_exception
    void insert(const KEY &key, const VALUE &val)
    {
        auto new_node_ptr = std::make_unique<node_t>(key, val);
        auto node_desc = find_node_desc(key);
        insert_by_hint(std::move(new_node_ptr), node_desc);
    }

    // throws bst_key_exists_exception
    template<class KIT, class VIT>
    void insert(KIT kbegin, KIT kend, VIT vbegin)
    {
        // Create all nodes, possibly causing KEY's constructor to throw.
        std::vector<std::unique_ptr<node_t>> nodes_to_add;
        for (; kbegin != kend; ++kbegin, ++vbegin)
        {
            nodes_to_add.push_back(std::make_unique<node_t>(*kbegin, *vbegin));
        }

        // Now add nodes. Operator< can throw exception, so we need to keep track
        // of places where we add.
        // Also, exception can be thrown when duplicate keys are inserted using this
        // function.
        std::vector<node_position_desc> position_descriptors(nodes_to_add.size());
        size_t num_commited = 0;

        try 
        {
            for (auto& node: nodes_to_add)
            {
                // This call can throw in KEY::operator<
                // `desc`<0> will either point to a leaf (nullptr)
                // or to an existing node.
                // `desc`<1> will be an already added node (commited now or added before insert).
                auto desc = find_node_desc(node->key());
                // This call can throw bst_key_exists
                insert_by_hint(std::move(node), desc);
                
                // If all succeds, "commit" this insert. No exceptions here.
                // (this was not the case when I was doing `position_descriptors.push_back(desc)` - it 
                // could bad_alloc on vector reallocation).
                position_descriptors[num_commited++] = desc;
            }
        }
        catch(...)
        {
            // Delete performed inserts.
            for (; num_commited > 0; --num_commited)
            {
                node_t* containing_node;
                node_t* parent;
                bool last_turn_was_left;

                std::tie(containing_node, parent, last_turn_was_left) = position_descriptors[num_commited - 1];

                if (parent == nullptr)
                {
                    root_ = nullptr;
                    continue;
                }

                if (last_turn_was_left)
                {
                    parent->takeLeft();
                }
                else
                {
                    parent->takeRight();
                }
            }

            throw;
        }
    }

    // throws bst_key_absent_exception
    VALUE& find(const KEY &key)
    {
        node_t* containing_node = std::get<0>(find_node_desc(key));

        if (containing_node == nullptr)
        {
            throw bst_key_absent_exception();
        }

        return containing_node->value();
    }

    bool contains(const KEY &key)
    {
        return std::get<0>(find_node_desc(key)) != nullptr;
    }

private:
    node_position_desc find_node_desc(const KEY& key)
    {
        node_t* cur_root = root_.get();
        // I don't use node.parent, because it requires to care for 
        // not dereferncing the nullptr.
        node_t* cur_parent = nullptr; 

        bool last_turn_was_left = false;        

        while (cur_root)
        {
            bool cur_key_less = cur_root->key() < key;
            bool cur_key_greater = key < cur_root->key();

            if (not cur_key_less and not cur_key_greater)
            {
                break;
            }

            cur_parent = cur_root;
            if (cur_key_less)
            {
                cur_root = cur_root->right(); 
                last_turn_was_left = false;
            }
            else
            {
                cur_root = cur_root->left();
                last_turn_was_left = true;
            }
        }

        return std::make_tuple(cur_root, cur_parent, last_turn_was_left);
    }


    // This function inserts node to a given position, assuming that we need
    // to insert it (i.e. it is not duplicate and position is valid, but may be already occupied).
    void insert_by_hint(std::unique_ptr<node_t>&& new_node_ptr, node_position_desc const& node_desc)
    {
        node_t* parent;
        node_t* containing_node;
        bool last_turn_was_left;
        
        std::tie(containing_node, parent, last_turn_was_left) = node_desc;

        if (containing_node != nullptr)
        {
            throw bst_key_exists_exception_detailed(new_node_ptr->key());
        }

        if (parent == nullptr)
        {
            root_ = std::move(new_node_ptr);
            return;
        }

        if (last_turn_was_left)
        {
            parent->putLeft(std::move(new_node_ptr));
        }
        else
        {
            parent->putRight(std::move(new_node_ptr));
        }
    }


    std::unique_ptr<node_t> root_;
};
