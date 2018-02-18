#pragma once

#include <cstddef>
#include <cstring>

struct node
{
    int key;
    node* next;
};

void add_to_list(node** head, node** tail, node** elem)
{
    if (*head == nullptr)
    {
        *head = *elem;
        *tail = *elem;
    }
    else
    {
        (*tail)->next = *elem;
        *tail = *elem;
    }
    *elem = (*elem)->next;
}

inline node* mergesort(node* input, size_t size)
{
    if (size <= 1)
    {
        return input;
    }

    size_t size_left = size / 2;
    size_t size_right = size - size_left;

    node* left_part = input;
    node* right_part = input;

    for (size_t i = 0; i < size_left; i++)
    {
        right_part = right_part->next;
    }

    left_part = mergesort(left_part, size_left);
    right_part = mergesort(right_part, size_right);

    size_t cur_idx_right = 0;
    size_t cur_idx_left = 0;

    node* sorted_head = nullptr;
    node* sorted_last = nullptr;

    while (cur_idx_left < size_left and
            cur_idx_right < size_right)
    {
        if (left_part->key <= right_part->key)
        {
            add_to_list(&sorted_head, &sorted_last, &left_part);
            ++cur_idx_left;
        }
        else 
        {
            add_to_list(&sorted_head, &sorted_last, &right_part);
            ++cur_idx_right;
        }
    }

    while (cur_idx_left < size_left)
    {
        add_to_list(&sorted_head, &sorted_last, &left_part);
        ++cur_idx_left;
    }

    while (cur_idx_right < size_right)
    {
        add_to_list(&sorted_head, &sorted_last, &right_part);
        ++cur_idx_right;
    }

    return sorted_head;
}
