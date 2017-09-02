#pragma once

#include <cstdint>
#include <cstddef>


struct my_vector_int
{
    struct vector_assignment_proxy
    {
        operator uint32_t () const;
        vector_assignment_proxy& operator=(uint32_t val);

    private:
        vector_assignment_proxy(size_t idx, my_vector_int& vec);
        size_t idx_;
        my_vector_int& vec_;

        friend struct my_vector_int;
    };

    my_vector_int(my_vector_int const&);
    my_vector_int();
    my_vector_int& operator=(my_vector_int);
    ~my_vector_int();

    void push_back(uint32_t val);
    void pop_back();
    vector_assignment_proxy operator[](size_t idx);
    uint32_t operator[](size_t idx) const;
    size_t size() const;

    void swap(my_vector_int& other);

private:
    void extend_buffer();
    void detach_buffer();

    static const int initial_capacity_ = 10;

    uint32_t *buffer_;
    size_t *counter_;
    size_t size_, capacity_;
};
