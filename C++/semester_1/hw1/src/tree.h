#pragma once
#include <fstream>

#pragma pack(8)
class tree_node
{
    int weight_;
    bool is_terminal_;
    char original_symbol_;

    tree_node* left_child_;
    tree_node* right_child_;

public:
    int get_weight() const;
    char get_original_symbol() const;
    bool is_terminal() const;
    tree_node* get_right_child() const;
    tree_node* get_left_child() const;

    tree_node(int weight, char orig_symbol);
    tree_node(int weight, tree_node* left_child, tree_node* right_child);
};

void delete_tree(tree_node *);
void flatten_tree(tree_node* root, tree_node** flattened_array, int& current_size);
int write_flattened_tree(tree_node**, int, std::ofstream&, int);
tree_node* create_tree_from_leafs(tree_node**, int);
tree_node* read_serialized_tree(std::ifstream&, int&);
