#include "any.h"

#include <utility>


namespace utils
{


any::any()
    : obj_(nullptr)
{
}

any::any(any const& other)
    : obj_(other.empty() ? nullptr : other.obj_->clone())
{
}

bool any::empty() const
{
    return obj_ == nullptr;
}

const std::type_info& any::type() const
{
    if (empty())
    {
        return typeid(void);
    }

    return obj_->type();
}

void any::swap(any& other)
{
    std::swap(obj_, other.obj_);
}

any& any::operator=(any other)
{
    swap(other);
    return *this;
}

any::~any()
{
    delete obj_;
}

void swap(any& x, any& y)
{
    x.swap(y);
}

} // namespace utils
