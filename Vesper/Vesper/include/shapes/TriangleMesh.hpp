#pragma once

#include <vector>

#include <shapes/Shape.hpp>

namespace vesp
{
    class TriangleMesh : public Shape
    {
    public:
        TriangleMesh();
        virtual ~TriangleMesh();

        //Bounding Box

        //Centroid

        //virtual void configure() = 0;

        virtual size_t getNumTriangles() const;
        virtual size_t getNumVertices() const;

        virtual const std::vector<Point3f>& getVertexPositions() const;
        virtual const std::vector<Point3i>& getTriangleIndices() const;


    protected:
        std::vector<Point3f> m_positions;
        std::vector<Normal3f> m_normals;
        std::vector<Point2f> m_texCoords;
        std::vector<Point3i> m_faces;
    };
}