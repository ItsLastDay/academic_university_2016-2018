#include "tree.h"

int tree_node::get_weight() const
{
    return weight_;
}

bool tree_node::is_terminal() const
{
    return is_terminal_;
}

char tree_node::get_original_symbol() const
{
    return original_symbol_;
}

tree_node* tree_node::get_right_child() const
{
    return right_child_;
}

tree_node* tree_node::get_left_child() const
{
    return left_child_;
}

tree_node::tree_node(int weight, char orig_symbol)
    : weight_(weight)
    , is_terminal_(true)
    , original_symbol_(orig_symbol)
    , left_child_(nullptr)
    , right_child_(nullptr)
{
}

tree_node::tree_node(int weight, tree_node* left_child, tree_node* right_child)
    : weight_(weight)
    , is_terminal_(false)
    , original_symbol_(-1)
    , left_child_(left_child)
    , right_child_(right_child)
{
}

tree_node* read_serialized_tree(std::ifstream& inp, int& tree_binary_size)
{   
    int node_count;
    inp.read(reinterpret_cast<char *>(&node_count), sizeof(node_count));
    tree_binary_size += sizeof(int);


    tree_node** flattened_tree = new tree_node* [node_count];

    for (int i = 0; i < node_count; i++) 
    {
        char orig_symbol;
        bool is_terminal;
        unsigned short left_child_num, right_child_num;

        inp.read(reinterpret_cast<char *>(&is_terminal), sizeof(is_terminal));
        inp.read(reinterpret_cast<char *>(&orig_symbol), sizeof(orig_symbol));
        inp.read(reinterpret_cast<char *>(&left_child_num), sizeof(left_child_num));
        inp.read(reinterpret_cast<char *>(&right_child_num), sizeof(right_child_num));

        if (is_terminal)
        {
            flattened_tree[i] = new tree_node(0, orig_symbol);
        } 
        else
        {
            auto left_child = flattened_tree[left_child_num];
            auto right_child = flattened_tree[right_child_num];
            flattened_tree[i] = new tree_node(0, left_child, right_child);
        }

        tree_binary_size += sizeof(is_terminal) + sizeof(orig_symbol) + sizeof(left_child_num)
            + sizeof(right_child_num);
    }

    tree_node* root = flattened_tree[node_count - 1];
    delete[] flattened_tree;
    return root;
}

int write_flattened_tree(tree_node** flattened_tree, int node_count, std::ofstream& out, 
        int input_size)
{
    int tree_binary_size = sizeof(input_size);
    out.write(reinterpret_cast<const char*>(&input_size), sizeof(input_size));
    if (input_size == 0)
    {
        return tree_binary_size;
    }

    tree_binary_size += sizeof(node_count);
    out.write(reinterpret_cast<const char*>(&node_count), sizeof(node_count));

    for (int i = 0; i < node_count; i++) 
    {
        char orig_symbol = flattened_tree[i]->get_original_symbol();
        bool is_terminal = flattened_tree[i]->is_terminal();
        unsigned short left_child_num = -1;
        unsigned short right_child_num = -1;

        for (int j = 0; j < i; j++)
        {
            if (flattened_tree[j] == flattened_tree[i]->get_left_child())
            {
                left_child_num = j;
            }

            if (flattened_tree[j] == flattened_tree[i]->get_right_child())
            {
                right_child_num = j;
            }
        }

        out.write(reinterpret_cast<const char *>(&is_terminal), sizeof(is_terminal));
        out.write(reinterpret_cast<const char *>(&orig_symbol), sizeof(orig_symbol));
        out.write(reinterpret_cast<const char *>(&left_child_num), sizeof(left_child_num));
        out.write(reinterpret_cast<const char *>(&right_child_num), sizeof(right_child_num));

        tree_binary_size += sizeof(is_terminal) + sizeof(orig_symbol) + sizeof(left_child_num)
            + sizeof(right_child_num);
    }

    return tree_binary_size;
}

tree_node* create_tree_from_leafs(tree_node** arr_nodes, int sz_arr_nodes)
{
    while (sz_arr_nodes > 1)
    {
        int index_of_min = 0;
        for (int i = 1; i < sz_arr_nodes; i++) 
        {
            if (arr_nodes[i]->get_weight() < 
                    arr_nodes[index_of_min]->get_weight())
            {
                index_of_min = i;
            }
        }

        int index_of_second_min = index_of_min ? 0 : 1;
        for (int i = 0; i < sz_arr_nodes; i++) 
        {
            if (i != index_of_min and
                    arr_nodes[i]->get_weight() < 
                    arr_nodes[index_of_second_min]->get_weight())
            {
                index_of_second_min = i;
            }
        }

        if (index_of_min < index_of_second_min)
        {
            std::swap(index_of_min, index_of_second_min);
        }

        std::swap(arr_nodes[sz_arr_nodes - 1], arr_nodes[index_of_min]);
        std::swap(arr_nodes[sz_arr_nodes - 2], arr_nodes[index_of_second_min]);

        sz_arr_nodes--;

        int combined_weight = arr_nodes[sz_arr_nodes - 1]->get_weight() + 
                    arr_nodes[sz_arr_nodes]->get_weight();
        arr_nodes[sz_arr_nodes - 1] = new tree_node(
                combined_weight,
                arr_nodes[sz_arr_nodes - 1],
                arr_nodes[sz_arr_nodes]);
    }

    return arr_nodes[0];
}

void flatten_tree(tree_node* root, tree_node** flattened_array, int& current_node_count)
{
    flattened_array[current_node_count] = root;
    current_node_count++;

    if (root->is_terminal())
    {
        return;
    }

    flatten_tree(root->get_left_child(), flattened_array, current_node_count);
    flatten_tree(root->get_right_child(), flattened_array, current_node_count);
}


void delete_tree(tree_node* root)
{
    if (root == nullptr)
    {
        return;
    }

    delete_tree(root->get_left_child());
    delete_tree(root->get_right_child());
    delete root;
}

