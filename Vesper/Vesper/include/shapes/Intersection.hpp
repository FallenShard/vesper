#pragma once

#include <math/Types.hpp>
#include <math/Frame.hpp>

namespace vesp
{
    class Shape;

    struct Intersection
    {
        // Intersection point
        Point3f p;

        // Unoccluded distance the ray traveled
        float tHit;

        // UV texture coordinates of the hit
        Point2f uv;

        // Shading frame, formed by interpolated normal
        Frame shFrame;

        // Geometric frame, formed from surface orientation 
        Frame geoFrame;

        // Pointer to the underlying shape
        const Shape* shape;

        Intersection()
            : shape(nullptr)
        {
        }

        Vector3f toLocal(const Vector3f& dir) const
        {
            return shFrame.toLocal(dir);
        }

        Vector3f toWorld(const Vector3f& dir) const
        {
            return shFrame.toWorld(dir);
        }
    };
}