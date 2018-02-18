#pragma once

#include <iostream>

struct point;
struct rectangle;
struct circle;


template<typename T, typename U>
void intersect(T &left, U &right)
{
   std::cout << "Intersecting "
       << typeid(T).name() << " and "
       << typeid(U).name()
       << std::endl;
}

struct shape
{
   shape(double x, double y)
       : x_(x)
       , y_(y)
   {}

   virtual void intersect(shape& other) = 0;

   virtual void intersect_impl(point &other) = 0;
   virtual void intersect_impl(rectangle &other) = 0;
   virtual void intersect_impl(circle &other) = 0;
protected:
   double x_;
   double y_;
};


struct point : shape
{
   point(double x, double y)
       : shape(x, y)
   {}

   void intersect(shape& other) override
   {
       other.intersect_impl(*this);
   }

   void intersect_impl(point &other) override
   {
       ::intersect(other, *this);
   }
   void intersect_impl(rectangle &other) override
   {
       ::intersect(other, *this);
   }
   void intersect_impl(circle &other) override
   {
       ::intersect(other, *this);
   } 
};


struct rectangle : shape
{
   rectangle(double left_x, double top_y,
       double width, double height)
       : shape(left_x, top_y)
       , width_(width)
       , height_(height)
   {}

   void intersect(shape& other) override
   {
       other.intersect_impl(*this);
   }

   void intersect_impl(point &other) override
   {
       ::intersect(other, *this);
   }
   void intersect_impl(rectangle &other) override
   {
       ::intersect(other, *this);
   }
   void intersect_impl(circle &other) override
   {
       ::intersect(other, *this);
   } 
private:
   double width_;
   double height_;
};


struct circle : shape
{
   circle(double center_x, double center_y, double radius)
       : shape(center_x, center_y)
   {}



   void intersect(shape &other) override 
   {
       other.intersect_impl(*this);
   }

   void intersect_impl(point &other) override
   {
       ::intersect(other, *this);
   }
   void intersect_impl(rectangle &other) override
   {
       ::intersect(other, *this);
   }
   void intersect_impl(circle &other) override
   {
       ::intersect(other, *this);
   } 
private:
   double radius_;
};


void intersect(shape &sh1, shape &sh2)
{
    sh1.intersect(sh2);
}

