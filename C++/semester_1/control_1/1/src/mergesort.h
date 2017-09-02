#pragma once

#include <cstddef>
#include <cstring>

inline void mergesort(int* array, size_t size)
{
    if (size <= 1)
    {
        return;
    }

    size_t size_left = size / 2;
    size_t size_right = size - size_left;

    int *left_part = new int[size_left];
    int *right_part = new int[size_right];

    memcpy(left_part, array, sizeof(int) * size_left);
    memcpy(right_part, array + size_left, sizeof(int) * size_right);

    mergesort(left_part, size_left);
    mergesort(right_part, size_right);

    size_t cur_idx_right = 0;
    size_t cur_idx_left = 0;

    while (cur_idx_left < size_left and 
            cur_idx_right < size_right)
    {
        if (left_part[cur_idx_left] <= right_part[cur_idx_right])
        {
            array[cur_idx_left + cur_idx_right] = left_part[cur_idx_left];
            ++cur_idx_left;
        }
        else
        {
            array[cur_idx_left + cur_idx_right] = right_part[cur_idx_right];
            ++cur_idx_right;
        }
    }

    while (cur_idx_left < size_left)
    {
        array[cur_idx_left + cur_idx_right] = left_part[cur_idx_left];
        ++cur_idx_left;
    }

    while (cur_idx_right < size_right)
    {
        array[cur_idx_left + cur_idx_right] = right_part[cur_idx_right];
        ++cur_idx_right; 
    }

    delete[] left_part;
    delete[] right_part;
}
