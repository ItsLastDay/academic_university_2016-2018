#pragma once

#include <cstddef>
#include <utility>

struct shared_buffer {
    explicit shared_buffer(size_t size);
    shared_buffer(const shared_buffer &src);
    shared_buffer& operator=(shared_buffer src);
    ~shared_buffer();
    char* get_data();
    const char* get_data() const;
    size_t get_size() const;

    friend void swap(shared_buffer&, shared_buffer&);

private:
    char* string_buf_;
    int* counter_;
    size_t size_;
};
