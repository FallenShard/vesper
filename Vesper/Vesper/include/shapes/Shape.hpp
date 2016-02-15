#pragma once

#include <math/Types.hpp>
#include <math/Frame.hpp>

namespace vesp
{
    class GeometryVisitor;

    struct Intersection
    {
        Point3f p;

        float t;

        Point2f uv;

        Frame shFrame;

        unsigned int triId;
        unsigned int geomId;
    };

    class Shape
    {
    public:
        virtual unsigned int submitGeometry(GeometryVisitor* visitor) = 0;
    };
}