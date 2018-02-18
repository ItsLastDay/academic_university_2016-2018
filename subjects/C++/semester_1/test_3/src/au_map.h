#pragma once

#include <cstddef>
#include <functional>
#include <iostream>

namespace containers {
    template<typename Key, typename Value, typename EQ=std::equal_to<Key>>
    struct au_map {
        typedef Key key_type;
        typedef Value value_type;
        typedef value_type& reference;
        typedef value_type const& const_reference;
        typedef value_type* pointer;
        typedef value_type const* const_pointer;
        typedef size_t size_type;

        struct node {
            node(node* next, const key_type& key, const_reference value)
                    : key_(key), value_(value), next_(next) {
            }

            reference get_value() {
                return value_;
            }

            const_reference get_value() const {
                return value_;
            }

            const key_type& get_key() const {
                return key_;
            }

            void set_value(const_reference value) {
                value_ = value;
            }

            node* get_next() {
                return next_;
            }

            const node* get_next() const {
                return next_;
            }

        private:
            Key key_;
            Value value_;
            node* next_;

            friend struct au_map;
        };

        au_map()
                : begin_(nullptr)
                , end_(nullptr)
                , size_(0)
                , eq_(EQ()) {
        }

        au_map(EQ eq)
                : begin_(nullptr)
                , end_(nullptr)
                , size_(0)
                , eq_(eq) {
        }

        au_map(const au_map& other)
                : begin_(nullptr)
                , end_(nullptr)
                , size_(0)
                , eq_(other.eq_) {
            for (const node* it = other.begin(); it != other.end(); it = it->get_next()) {
                insert(it->get_key(), it->get_value());
            }
        }

        ~au_map() {
            clear();
        }

        node* insert(const key_type& key, const_reference value) {
            if (find(key) == nullptr) {
                ++size_;
                begin_ = new node(begin(), key, value);
                return begin_;
            }

            return nullptr;
        }

        node* find(const key_type& key) {
            for (node* it = begin(); it != end(); it = it->get_next()) {
                if (eq_(it->get_key(), key)) {
                    return it;
                }
            }

            return nullptr;
        }

        void erase(node* position) {
            node* it = begin();
            if (it != position) {
                while (it->get_next() != position) {
                    it = it->get_next();
                };

                it->next_ = position->next_;
            } else {
                begin_ = begin_->next_;
            }

            --size_;
            delete position;
        }

        node* begin() {
            return begin_;
        }

        const node* begin() const {
            return begin_;
        }

        node* end() {
            return end_;
        }

        const node* end() const {
            return end_;
        }

        bool empty() const {
            return size_ == 0;
        }

        size_type size() const {
            return size_;
        }

        void clear() {
            while (!empty()) {
                erase(begin());
            }
        }

    private:
        node* begin_;
        node* end_;
        size_type size_;
        EQ eq_;
    };
}