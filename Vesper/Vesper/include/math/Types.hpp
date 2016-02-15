#pragma once

#include <math/Vector.hpp>
#include <math/Point.hpp>
#include <math/Normal.hpp>
#include <math/Ray.hpp>

namespace vesp
{
    using Vector2f = Vector<float, 2>;
    using Vector3f = Vector<float, 3>;
    using Vector4f = Vector<float, 4>;

    using Vector2i = Vector<int, 2>;
    
    using Point2f = Point<float, 2>;
    using Point3f = Point<float, 3>;

    using Point2i = Point<int, 2>;
    using Point3i = Point<int, 3>;

    using Point3ui = Point<unsigned int, 3>;

    using Normal3f = Normal<float, 3>;

    using Ray3f = Ray<Point3f, Vector3f>;
}