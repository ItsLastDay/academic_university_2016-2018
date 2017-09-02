#include <iostream>
#include <cstring>

char *read_string_until_endl()
{
    const int initial_buf_size = 2;
    char *buf = new char[initial_buf_size];

    int pos_to_read = 0;
    int current_size = initial_buf_size; 
    
    while (true)
    {
        std::cin.getline(buf + pos_to_read, 
                current_size - pos_to_read, '\n');
        if (not std::cin.fail())
        {
            break;
        }
        std::cin.clear();

        buf[current_size - 1] = 0;

        pos_to_read = current_size - 1;

        int larger_size = current_size * 1.5;
        char *larger_buf = new char[larger_size];
        memcpy(larger_buf, buf, current_size);
        
        delete[] buf;
        buf = larger_buf;
        current_size = larger_size;
    }

    return buf;
}

void print_search_url(char *search_string)
{
    std::cout << "https://www.google.ru/search?q=";
    while (*search_string)
    {
        if (*search_string == ' ')
        {
            std::cout << "%20";
        } 
        else
        {
            std::cout << *search_string;
        }

        ++search_string;
    }

    std::cout << std::endl;
}

void print_best_pal(char *str)
{
    unsigned int str_size = strlen(str);
    int largest_pal_length = 0;
    int largest_pal_start = -1;

    for (unsigned int i = 0; i < str_size; i++)
    {
        for (unsigned int j = i + largest_pal_length; j < str_size; j++)
        {
            bool is_pal = true;
            for(unsigned int k = i; k <= j; k++)
            {
                is_pal &= str[k] == str[j - (k - i)];
            }

            if (is_pal)
            {
                largest_pal_length = j - i + 1;
                largest_pal_start = i;
            }
        }
    }

    str[largest_pal_length + largest_pal_start] = 0;
    std::cout << (str + largest_pal_start) << std::endl;
} 

void print_memory(void *memory_pointer, int memory_size)
{
    auto char_memory_pointer = static_cast<char *>(memory_pointer);
    while (memory_size)
    {
        std::cout << *char_memory_pointer;
        memory_size--;
        char_memory_pointer++;
    }
}

int main()
{
    using namespace std;
    int current_result = 0;
    while (true)
    {
        char command;
        cin >> command;

        if (cin.eof())
        {
            break;
        }

        switch (command)
        {
            case 'a':
            {
                int addend;
                cin >> addend;
                current_result += addend;         
                break;
            }
            case 'f':
            {
                double addend_dbl;
                cin >> addend_dbl;
                current_result += static_cast<int>(addend_dbl);
                break;
            }
            case 's':
            {
                int minuend;
                cin >> minuend;
                current_result -= minuend;
                break;
            }
            case 'r':
            {
                cout << "<< " << current_result << endl;
                break;
            }
            case 'g':
            {
                cin.ignore(1); // Ignore the whitespace between 'g' and str.
                char *query_string = read_string_until_endl();
                print_search_url(query_string);
                delete[] query_string;
                break;
            }
            case 'p':
            {
                cin.ignore(1);
                char *input_str = read_string_until_endl();
                print_best_pal(input_str);
                delete[] input_str;
                break;
            }
            case 'm':
            {
                void *memory_pointer = 0;
                int memory_size;
                cin >> hex >> memory_pointer >> memory_size;
                print_memory(memory_pointer, memory_size);
                break;
            }
            default:
            {
                cout << "Unknown command!" << endl;
                return 1;
            }
        }
    }

    cout << "Bye!" << endl;
    return 0;
}
