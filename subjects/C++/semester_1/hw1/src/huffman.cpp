#include <iostream>
#include <limits>
#include <cstring>
#include "tree.h"

namespace
{
    using huffman_mode = enum { none, compress, decompress };

    void init_settings_failed()
    {
        std::cerr << "Invalid arguments. Exiting." << std::endl;
        exit(1);
    }

    struct huffman_settings
    {
        huffman_mode mode;
        char* inp_file_name;
        char* out_file_name;

        huffman_settings(int argc, char** argv)
            : mode(none)
            , inp_file_name(nullptr)
            , out_file_name(nullptr)
        {
            for (int i = 0; i < argc; i++)
            {
                if (strcmp(argv[i], "-c") == 0)
                {
                    if (mode != none)
                    {
                        init_settings_failed();
                    }
                    mode = compress;
                }
                if (strcmp(argv[i], "-u") == 0)
                {
                    if (mode != none)
                    {
                        init_settings_failed();
                    }
                    mode = decompress;
                }

                if (strcmp(argv[i], "-f") == 0 or strcmp(argv[i], "--file") == 0)
                {
                    if (inp_file_name != nullptr)
                    {
                        init_settings_failed();
                    }
                    ++i;
                    inp_file_name = argv[i];
                }

                if(strcmp(argv[i], "-o") == 0 or strcmp(argv[i], "--output") == 0)
                {
                    if (out_file_name != nullptr)
                    {
                        init_settings_failed();
                    }
                    ++i;
                    out_file_name = argv[i];
                }
            }

            if (mode == none or inp_file_name == nullptr or out_file_name == nullptr)
            {
                init_settings_failed();
            }
        }
    };


    class huffman_algo
    {
        huffman_settings settings_;
        const int max_char_value = std::numeric_limits<unsigned char>::max() + 1;


        tree_node** flatten_tree_arr(tree_node* root, int& total_size) const
        {
            tree_node** flattened_tree = new tree_node* [2 * max_char_value];

            flatten_tree(root, flattened_tree, total_size);
            for (int i = 0; i < total_size - i - 1; i++)
            {
                std::swap(flattened_tree[i], flattened_tree[total_size - i - 1]);
            }

            return flattened_tree;
        }

        int* create_freq_count(std::ifstream& inp, int& input_size) const
        {
            char current_character;
            int* frequency_count = new int [max_char_value]();

            while (inp.get(current_character))
            {
                input_size++;
                frequency_count[static_cast<unsigned char>(current_character)]++;
            }

            return frequency_count;
        }

        tree_node** get_leaf_nodes(int* frequency_count, int& effective_leaf_nodes_size) const
        {
            tree_node** leaf_nodes = new tree_node* [max_char_value];
            for (int i = 0; i < max_char_value; i++) 
            {
                if (frequency_count[i] != 0)
                {
                    leaf_nodes[effective_leaf_nodes_size] = 
                        new tree_node(frequency_count[i], static_cast<char>(i));
                    effective_leaf_nodes_size++;
                }
            }

            return leaf_nodes;
        }

        std::pair<unsigned int, short>* 
        precompute_huffman_code(tree_node** flattened_tree, int node_count) const
        {
            auto huffman_codes = new std::pair<unsigned int, short> [max_char_value];

            for (int i = 0; i < max_char_value; i++) 
            {
                unsigned int current_code = 0;
                int current_code_length = 0;

                int current_node_idx = node_count;
                for (int j = 0; j < node_count; j++)
                {
                    if (flattened_tree[j]->is_terminal() and 
                            flattened_tree[j]->get_original_symbol() == static_cast<char>(i))
                    {
                        current_node_idx = j;
                        break;
                    }
                }
                if (current_node_idx == node_count)
                {
                    // Never saw such symbol in input.
                    continue;
                }

                while (current_node_idx != node_count - 1)
                {
                    for (int j = current_node_idx + 1; j < node_count; j++)
                    {
                        if (flattened_tree[j]->get_right_child() == 
                                flattened_tree[current_node_idx])
                        {
                            current_node_idx = j;
                            current_code |= (1u << current_code_length);
                            current_code_length++;
                            break;
                        }

                        if (flattened_tree[j]->get_left_child() ==
                                flattened_tree[current_node_idx])
                        {
                            current_node_idx = j;
                            current_code_length++;
                            break;
                        }
                    }
                }

                for (int j = 0; j < current_code_length - j - 1; j++)
                {
                    unsigned int val_j = (current_code >> j) & 1;
                    unsigned int val_j_mirror = (current_code >> (current_code_length - j - 1)) & 1;

                    unsigned mask_now = (val_j << j) | (val_j_mirror << (current_code_length - j - 1));
                    current_code ^= mask_now;

                    unsigned mask_want = (val_j_mirror << j) | (val_j << (current_code_length - j - 1));
                    current_code ^= mask_want;
                }


                current_code_length = std::max(current_code_length, 1); 

                huffman_codes[i] = std::make_pair(current_code, current_code_length);
            }

            return huffman_codes;
        }

        int write_encoded_data(std::ifstream& inp, std::ofstream& out,
                std::pair<unsigned, short>* codes, int input_size) const
        {
            auto output_buffer = new char [2 * input_size]();
            auto input_buffer = new char [input_size];
            
            inp.clear();
            inp.seekg(0);
            inp.read(input_buffer, input_size);

            int current_output_idx = 0;
            short current_output_shift = 0;

            for (int i = 0; i < input_size; i++) 
            {
                auto code_and_length = codes[static_cast<unsigned char>(input_buffer[i])];
                unsigned code = code_and_length.first;
                short code_length = code_and_length.second;

                for (int j = 0; j < code_length; j++)
                {
                    output_buffer[current_output_idx] |=
                        ((code >> j) & 1) << current_output_shift;
                    current_output_shift++;
                    if (current_output_shift == sizeof(char) * 8)
                    {
                        current_output_shift = 0;
                        current_output_idx++;
                    }
                }
            }

            current_output_idx += current_output_shift != 0;

            out.write(output_buffer, current_output_idx);

            delete[] input_buffer;
            delete[] output_buffer;

            return current_output_idx * sizeof(char);
        }

        int 
        write_decoded_data(std::ifstream& inp, std::ofstream& out, tree_node* root, int output_size) const
        {
            auto output_buffer = new char[output_size];

            int num_read_bytes = 0;

            char current_input_char;
            short current_input_offset = 8 * sizeof(char);

            for (int i = 0; i < output_size; i++)
            {
                tree_node* current_node = root;

                while (not current_node->is_terminal())
                {
                    if (current_input_offset == 8 * sizeof(char))
                    {
                        current_input_offset = 0;
                        inp.get(current_input_char);
                        num_read_bytes++;
                    }

                    if (current_input_char & (1 << current_input_offset))
                    {
                        current_node = current_node->get_right_child();
                    }
                    else
                    {
                        current_node = current_node->get_left_child();
                    }

                    current_input_offset++;
                }

                output_buffer[i] = current_node->get_original_symbol();
            }

            out.write(output_buffer, output_size);

            delete[] output_buffer;

            return num_read_bytes;
        }

        void run_compress() const
        {
            std::ifstream inp(settings_.inp_file_name, std::ifstream::binary);
            std::ofstream out(settings_.out_file_name, std::ofstream::binary);

            int input_size = 0;
            auto frequency_count = create_freq_count(inp, input_size);

            int effective_leaf_nodes_size = 0;
            auto leaf_nodes = get_leaf_nodes(frequency_count, effective_leaf_nodes_size);

            auto root = create_tree_from_leafs(leaf_nodes, effective_leaf_nodes_size);

            int node_count = 0;
            auto flattened_tree = effective_leaf_nodes_size ? flatten_tree_arr(root, node_count) : nullptr;

            int tree_binary_size = write_flattened_tree(flattened_tree, node_count, out,
                    input_size);

            auto precomputed_codes = precompute_huffman_code(flattened_tree, node_count);

            int total_bytes_written = write_encoded_data(inp, 
                    out, precomputed_codes, input_size);

            std::cout << input_size * sizeof(char) << std::endl;
            std::cout << total_bytes_written << std::endl;
            std::cout << tree_binary_size << std::endl;
            delete[] leaf_nodes;
            delete[] flattened_tree;
            delete[] frequency_count;
            delete[] precomputed_codes;
            if (effective_leaf_nodes_size != 0) 
            {
                delete_tree(root);
            }
        }

        void run_decompress() const
        {
            std::ifstream inp(settings_.inp_file_name, std::ifstream::binary);
            std::ofstream out(settings_.out_file_name, std::ofstream::binary);

            int output_size;
            inp.read(reinterpret_cast<char *>(&output_size), sizeof(output_size));

            int tree_binary_size = sizeof(int);
            if (output_size != 0) 
            {
                auto root = read_serialized_tree(inp, tree_binary_size);

                int input_size = write_decoded_data(inp, out, root, output_size);

                std::cout << tree_binary_size + input_size * sizeof(char) << std::endl;
                std::cout << output_size << std::endl;
                std::cout << tree_binary_size << std::endl;

                delete_tree(root);
            }
            else
            {
                std::cout << tree_binary_size << std::endl;
                std::cout << 0 << std::endl;
                std::cout << tree_binary_size << std::endl;
            }
        }

    public:
        huffman_algo(huffman_settings const& settings)
            : settings_(settings)
        {
        }

        void run() const
        {
            switch (settings_.mode)
            {
                case compress:
                    run_compress();
                    break;
                case decompress:
                    run_decompress();
                    break;
                default:
                    exit(1);
            }
        }
    };
}

int main(int argc, char** argv)
{
    auto settings = huffman_settings(argc, argv);
    auto huffman = huffman_algo(settings);
    huffman.run();
    return 0;
}
