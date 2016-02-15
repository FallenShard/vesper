#include <shapes/TriangleMesh.hpp>

namespace vesp
{
    TriangleMesh::TriangleMesh()
    {
    }

    TriangleMesh::~TriangleMesh()
    {
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