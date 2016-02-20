#include <shapes/TriangleMesh.hpp>

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
        {
            its.uv = barycentric.x() * m_texCoords[std::get<2>(m_vertices[idx0])]
                   + barycentric.y() * m_texCoords[std::get<2>(m_vertices[idx1])]
                   + barycentric.z() * m_texCoords[std::get<2>(m_vertices[idx2])];
        }

        its.geoFrame = Frame((p1 - p0).cross(p2 - p0).normalized());

        if (m_normals.size() > 0)
        {
            its.shFrame = Frame(
                (barycentric.x() * m_normals[std::get<1>(m_vertices[idx0])] +
                 barycentric.y() * m_normals[std::get<1>(m_vertices[idx1])] +
                 barycentric.z() * m_normals[std::get<1>(m_vertices[idx2])]).normalized());
        }
        else
            its.shFrame = its.geoFrame;

        its.shape = this;
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
}