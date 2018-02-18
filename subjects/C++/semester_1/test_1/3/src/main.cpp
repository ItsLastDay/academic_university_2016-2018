#include <iostream>
#include "mergesort.h"
#include <cassert>
#include <fstream>
using namespace std;

node* read_input_numbers_list(ifstream &in, size_t &out_size)
{
    node* head_node = nullptr;
    node* cur_node = head_node;
    int current_number;
    out_size = 0;

    while (in >> current_number)
    {
        ++out_size;
        node* nx_node = new node({current_number, nullptr});
        if (head_node == nullptr)
        {
            head_node = nx_node;
            cur_node = nx_node;
        }
        else 
        {
            cur_node->next = nx_node;
            cur_node = nx_node;
        }
    }

    return head_node;
}


int main()
{
    ifstream in("input.txt");
    ofstream out("output.txt");

    size_t numbers_size;
    node* input_node = read_input_numbers_list(in, numbers_size);

    input_node = mergesort(input_node, numbers_size);
    
    for (size_t i = 0; i < numbers_size; i++)
    {
        node* next_node = input_node->next;
        out << input_node->key << " ";
        delete input_node;
        input_node = next_node;
    }

    return 0;
}
