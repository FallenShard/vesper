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

        virtual void configure();
        virtual void setIntersectionInfo(unsigned int triangleId, const Ray3f& ray, Intersection& its) const override;


        virtual size_t getNumTriangles() const;
        virtual size_t getNumVertices() const;

        virtual const std::vector<Point3f>& getVertexPositions() const;
        virtual const std::vector<Point3i>& getTriangleIndices() const;


    protected:
        std::vector<Point3f> m_positions;
        std::vector<Normal3f> m_normals;
        std::vector<Point2f> m_texCoords;
        std::vector<Point3i> m_faces;

        using Indices = std::tuple<unsigned int, unsigned int, unsigned int>;
        std::vector<Indices> m_vertices;
        std::vector<Indices> m_triangles;
    };
}