#include <shapes/TriangleMesh.hpp>
#include <sampling/Sampler.hpp>
#include <sampling/Warp.hpp>

namespace vesp
{
    TriangleMesh::TriangleMesh()
    {
    }

    TriangleMesh::~TriangleMesh()
    {
    }

    void TriangleMesh::configure()
    {
        Shape::configure();

        m_pdf.reserve(getNumTriangles());
        for (auto i = 0; i < getNumTriangles(); i++)
            m_pdf.append(triangleArea(i));
        m_pdf.normalize();
    }

    void TriangleMesh::setIntersectionInfo(unsigned int triangleId, const Ray3f& ray, Intersection& its) const
    {
        Vector3f barycentric;
        barycentric[0] = 1 - its.uv.x() - its.uv.y();
        barycentric[1] = its.uv.x();
        barycentric[2] = its.uv.y();

        unsigned int idx0 = std::get<0>(m_triangles[triangleId]);
        unsigned int idx1 = std::get<1>(m_triangles[triangleId]);
        unsigned int idx2 = std::get<2>(m_triangles[triangleId]);

        Point3f p0 = m_positions[std::get<0>(m_vertices[idx0])];
        Point3f p1 = m_positions[std::get<0>(m_vertices[idx1])];
        Point3f p2 = m_positions[std::get<0>(m_vertices[idx2])];

        its.p = barycentric.x() * p0 + barycentric.y() * p1 + barycentric.z() * p2;

        if (m_texCoords.size() > 0)
            its.uv = interpolateTexCoord(triangleId, barycentric);

        its.geoFrame = Frame((p1 - p0).cross(p2 - p0).normalized());

        if (m_normals.size() > 0)
            its.shFrame = Frame(interpolateNormal(triangleId, barycentric));
        else
            its.shFrame = its.geoFrame;

        its.shape = this;
    }

    void TriangleMesh::sampleSurface(ShapeSample& shapeSample, Sampler& sampler) const
    {
        Point2f sample = sampler.next2D();
        unsigned int triangleId = static_cast<unsigned int>(m_pdf.sampleReuse(sample.x()));

        Vector3f barycentric = Warp::squareToUniformTriangle(sample);

        shapeSample.p = interpolatePosition(triangleId, barycentric);
        if (m_normals.size() > 0)
            shapeSample.n = interpolateNormal(triangleId, barycentric);
        else
        {
            const Point3f p0 = m_positions[std::get<0>(m_vertices[std::get<0>(m_triangles[triangleId])])];
            const Point3f p1 = m_positions[std::get<0>(m_vertices[std::get<1>(m_triangles[triangleId])])];
            const Point3f p2 = m_positions[std::get<0>(m_vertices[std::get<2>(m_triangles[triangleId])])];
            Normal3f n = (p1 - p0).cross(p2 - p0).normalized();
            shapeSample.n = n;
        }
        shapeSample.pdf = m_pdf.getNormFactor();
    }

    float TriangleMesh::pdfSurface(const ShapeSample& shapeSample) const
    {
        return m_pdf.getNormFactor();
    }

    size_t TriangleMesh::getNumTriangles() const
    {
        return m_faces.size();
    }

    size_t TriangleMesh::getNumVertices() const
    {
        return m_positions.size();
    }

    const std::vector<Point3f>& TriangleMesh::getVertexPositions() const
    {
        return m_positions;
    }
    const std::vector<Point3i>& TriangleMesh::getTriangleIndices() const
    {
        return m_faces;
    }

    float TriangleMesh::triangleArea(unsigned int triangleId) const
    {
        const Point3f p0 = m_positions[std::get<0>(m_vertices[std::get<0>(m_triangles[triangleId])])];
        const Point3f p1 = m_positions[std::get<0>(m_vertices[std::get<1>(m_triangles[triangleId])])];
        const Point3f p2 = m_positions[std::get<0>(m_vertices[std::get<2>(m_triangles[triangleId])])];

        return 0.5f * (p1 - p0).cross(p2 - p0).norm();
    }
    Point3f TriangleMesh::interpolatePosition(unsigned int triangleId, const Vector3f& barycentric) const
    {
        const Point3f p0 = m_positions[std::get<0>(m_vertices[std::get<0>(m_triangles[triangleId])])];
        const Point3f p1 = m_positions[std::get<0>(m_vertices[std::get<1>(m_triangles[triangleId])])];
        const Point3f p2 = m_positions[std::get<0>(m_vertices[std::get<2>(m_triangles[triangleId])])];
        return barycentric.x() * p0 + barycentric.y() * p1 + barycentric.z() * p2;
    }

    Normal3f TriangleMesh::interpolateNormal(unsigned int triangleId, const Vector3f& barycentric) const
    {
        const Normal3f n0 = m_normals[std::get<1>(m_vertices[std::get<0>(m_triangles[triangleId])])];
        const Normal3f n1 = m_normals[std::get<1>(m_vertices[std::get<1>(m_triangles[triangleId])])];
        const Normal3f n2 = m_normals[std::get<1>(m_vertices[std::get<2>(m_triangles[triangleId])])];
        return (barycentric.x() * n0 + barycentric.y() * n1 + barycentric.z() * n2).normalized();
    }

    Point2f TriangleMesh::interpolateTexCoord(unsigned int triangleId, const Vector3f& barycentric) const
    {
        const Point2f tc0 = m_texCoords[std::get<2>(m_vertices[std::get<0>(m_triangles[triangleId])])];
        const Point2f tc1 = m_texCoords[std::get<2>(m_vertices[std::get<1>(m_triangles[triangleId])])];
        const Point2f tc2 = m_texCoords[std::get<2>(m_vertices[std::get<2>(m_triangles[triangleId])])];
        return barycentric.x() * tc0 + barycentric.y() * tc1 + barycentric.z() * tc2;
    }
}