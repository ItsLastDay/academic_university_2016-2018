#pragma once

#include <iostream>
#include <functional>
#include <vector>
#include <typeinfo>
#include <typeindex>


struct shape
{
   shape(double x, double y)
       : x_(x)
       , y_(y)
   {}

   virtual ~shape() {}
protected:
   double x_;
   double y_;
};


struct point : shape
{
   point(double x, double y)
       : shape(x, y)
   {}

};


struct rectangle : shape
{
   rectangle(double left_x, double top_y,
       double width, double height)
       : shape(left_x, top_y)
       , width_(width)
       , height_(height)
   {}

private:
   double width_;
   double height_;
};


struct circle : shape
{
   circle(double center_x, double center_y, double radius)
       : shape(center_x, center_y)
   {}



private:
   double radius_;
};


template<class T, class ID>
struct double_dispatcher
{
    template<class ARG1, class ARG2>
    static void reg(std::function<void(ARG1, ARG2)> func)
    {
        auto new_func = [func](T& arg1, T& arg2)
        {
            func(static_cast<ARG1>(arg1), static_cast<ARG2>(arg2));
        };
        auto store_tuple = make_tuple(std::type_index(typeid(ARG1)), std::type_index(typeid(ARG2)), std::move(new_func));
        get_instance().funcs_.push_back(std::move(store_tuple));
    }

    static void call(T& first_arg, T& second_arg)
    {
        for (auto const& func_tuple: get_instance().funcs_)
        {
            if (std::type_index(typeid(first_arg)) == std::get<0>(func_tuple)
                    && std::type_index(typeid(second_arg)) == std::get<1>(func_tuple))
            {
                std::get<2>(func_tuple)(first_arg, second_arg);
                return;
            }
        }

        call(second_arg, first_arg);
    }

private:
    static double_dispatcher& get_instance()
    {
        static double_dispatcher instance;
        return instance;
    }

    std::vector<std::tuple<std::type_index, std::type_index, std::function<void(T&, T&)>>> funcs_;
};


void intersect_ci_po(circle &sh1, point &sh2)
{
    std::cout << "circle point" << std::endl;
}

void intersect_ci_re(circle &sh1, rectangle &sh2)
{
    std::cout << "circle rect" << std::endl;
}

void intersect_ci_ci(circle &sh1, circle &sh2)
{
    std::cout << "circle circle" << std::endl;
}

void intersect_po_po(point &sh1, point &sh2)
{
    std::cout << "point point" << std::endl;
}

void intersect_po_re(point &sh1, rectangle &sh2)
{
    std::cout << "point rectangle" << std::endl;
}

void intersect_re_re(rectangle &sh1, rectangle &sh2)
{
    std::cout << "rectangle rectangle" << std::endl;
}

void intersect(shape &left, shape &right)
{
    double_dispatcher<shape, int>::call(left, right);
}

void shape_init_intersect_dd()
{
    double_dispatcher<shape, int>::reg<circle&, point&>(intersect_ci_po);
    double_dispatcher<shape, int>::reg<circle&, rectangle&>(intersect_ci_re);
    double_dispatcher<shape, int>::reg<circle&, circle&>(intersect_ci_ci);
    double_dispatcher<shape, int>::reg<point&, point&>(intersect_po_po);
    double_dispatcher<shape, int>::reg<point&, rectangle&>(intersect_po_re);
    double_dispatcher<shape, int>::reg<rectangle&, rectangle&>(intersect_re_re);
}
