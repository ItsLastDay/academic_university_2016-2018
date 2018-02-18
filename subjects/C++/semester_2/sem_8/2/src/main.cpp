#include <vector>
#include <memory>

#include "shapes.h"

int main()
{
    shape_init_intersect_dd();

    std::vector<std::shared_ptr<shape>> shapes = {
        std::make_shared<point>(5, 3),
        std::make_shared<rectangle>(0, 0, 10, 10),
        std::make_shared<circle>(-1, -400, 40543)
    };

    for(auto shpptr1 : shapes)
       for(auto shpptr2 : shapes)
          intersect(*shpptr1, *shpptr2);
    return 0;
}
