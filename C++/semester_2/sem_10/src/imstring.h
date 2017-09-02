#pragma once

#include <ciso646>
#include <string>
#include <memory>
#include <ostream>


namespace detail
{

struct imstring_node
{
    imstring_node(const char* content)
        : content_(std::make_shared<std::string>(content))
        , is_leaf_(true)
    {
    }

    imstring_node(std::shared_ptr<imstring_node> le, std::shared_ptr<imstring_node> rg)
        : left_(le)
        , right_(rg)
        , is_leaf_(false)
    {
    }
private:
    std::shared_ptr<imstring_node> left_, right_;
    std::shared_ptr<std::string> content_;
    bool is_leaf_;
};

} // namespace detail

struct imstring
{
    imstring(imstring const&) = default;
    imstring(imstring &&) = default;

    imstring(const char* str)
        : buf_ptr_(std::make_shared<std::string>(str))
    {
    }

    imstring& operator=(imstring const&) = delete;
    imstring& operator=(imstring const&&) = delete;

    imstring operator+(imstring const& rhs) const
    {
        imstring result = imstring();
        result.root_ = std::make_shared(
    }

    friend std::ostream& operator<<(std::ostream& out, imstring const& str)
    {
        out << *(str.root_);
        return out;
    }

    size_t size() const
    {
        return root_->size();
    }

    char operator[](size_t idx) const
    {
        return (*root_)[idx];
    }

    const char* end() const
    {
        return c_str() + root->size();
    }

    const char* begin() const
    {
        return c_str();
    }

    const char* c_str() const
    {
        return root_->c_str();
    }

private:
    std::shared_ptr<detail::imstring_node> root_;
};
