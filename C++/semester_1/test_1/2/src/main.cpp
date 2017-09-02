#include <iostream>
#include "mergesort.h"
#include <cassert>
#include <fstream>
using namespace std;

namespace 
{
    const int initial_array_size = 10;
    const float array_expand_multiplier = 1.5;
}

int* read_input_numbers(ifstream &in, int &out_size)
{
    int allocated_numbers_size = initial_array_size;
    int real_number_size = 0;
    int* numbers = new int [allocated_numbers_size];
    int current_number;

    while (in >> current_number)
    {
        if (real_number_size == allocated_numbers_size)
        {
            int next_allocated_size = allocated_numbers_size * 
                array_expand_multiplier;
            int* next_numbers = new int [next_allocated_size];
            memcpy(next_numbers, numbers, sizeof(int) * allocated_numbers_size);
            allocated_numbers_size = next_allocated_size;
            delete[] numbers;
            numbers = next_numbers;
        }

        numbers[real_number_size] = current_number;
        ++real_number_size;
    }

    out_size = real_number_size;
    return numbers;
}

int main()
{
    ifstream in("input.txt");
    ofstream out("output.txt");

    int numbers_size;
    int* input_numbers = read_input_numbers(in, numbers_size);

    mergesort(input_numbers, numbers_size);
    for (int i = 0; i < numbers_size; i++)
    {
        out << input_numbers[i] << " ";
    }
    out << endl;

    delete[] input_numbers;
    return 0;
}
