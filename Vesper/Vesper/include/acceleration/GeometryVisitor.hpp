#pragma once

#include <shapes/TriangleMesh.hpp>

namespace vesp
{
    class GeometryVisitor
    {
    public:
        virtual unsigned int addTriangleMesh(TriangleMesh* mesh) = 0;
    };
}