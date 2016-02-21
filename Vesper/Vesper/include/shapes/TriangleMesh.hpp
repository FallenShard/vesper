#pragma once

#include <vector>

#include <shapes/Shape.hpp>
#include <sampling/DiscretePdf.hpp>

namespace vesp
{
    class TriangleMesh : public Shape
    {
    public:
        TriangleMesh();
        virtual ~TriangleMesh();

        virtual void configure() override;
        virtual void setIntersectionInfo(unsigned int triangleId, const Ray3f& ray, Intersection& its) const override;
        virtual void sampleSurface(ShapeSample& shapeSample, Sampler& sampler) const override;
        virtual float pdfSurface(const ShapeSample& shapeSample) const override;

        virtual size_t getNumTriangles() const;
        virtual size_t getNumVertices() const;

        virtual const std::vector<Point3f>& getVertexPositions() const;
        virtual const std::vector<Point3i>& getTriangleIndices() const;

        float triangleArea(unsigned int triangleId) const;

        Point3f interpolatePosition(unsigned int triangleId, const Vector3f& barycentric) const;
        Normal3f interpolateNormal(unsigned int triangleId, const Vector3f& barycentric) const;
        Point2f interpolateTexCoord(unsigned int triangleId, const Vector3f& barycentric) const;


    protected:
        std::vector<Point3f> m_positions;
        std::vector<Normal3f> m_normals;
        std::vector<Point2f> m_texCoords;
        std::vector<Point3i> m_faces;

        using Indices = std::tuple<unsigned int, unsigned int, unsigned int>;
        std::vector<Indices> m_vertices;
        std::vector<Indices> m_triangles;

        DiscretePdf m_pdf;
    };
}