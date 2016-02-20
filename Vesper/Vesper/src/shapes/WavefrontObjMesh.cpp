#include <unordered_map>

#include <acceleration/GeometryVisitor.hpp>


#include <shapes/WavefrontObjMesh.hpp>
#include <math/Transform.hpp>

namespace vesp
{
    WavefrontObjMesh::WavefrontObjMesh(const AttributeList& attributes)
    {
        std::string meshFilename = "res/" + attributes.getString("filename", "cube.obj");
        std::ifstream meshFile(meshFilename);
        if (meshFile.fail())
        {
            std::cout << "Could not open " + meshFilename << std::endl;
            return;
        }

        auto mat = Eigen::Translation<float, 3>(-0.f, -1.f, 5.f) * Eigen::DiagonalMatrix<float, 3>(Vector3f(.25, .25, .25)) * Eigen::Matrix4f::Identity();

        Transform trans(mat);

        std::cout << "Loading Wavefront Obj mesh: " + meshFilename << std::endl;

        using VertexMap = std::unordered_map<ObjVertex, unsigned int, ObjVertexHasher>;

        VertexMap vertexMap;
        std::vector<ObjVertex> vertexData;

        std::string line;
        while (std::getline(meshFile, line))
        {
            std::istringstream lineStream(line);

            std::string prefix;
            lineStream >> prefix;

            if (prefix == "v")
            {
                Point3f p;
                lineStream >> p.x() >> p.y() >> p.z();
                p = trans * p;
                m_positions.push_back({ p.x(), p.y(), p.z() });
            }
            else if (prefix == "vt")
            {
                Point2f tc;
                lineStream >> tc.x() >> tc.y();
                m_texCoords.push_back(tc);
            }
            else if (prefix == "vn")
            {
                Normal3f n;
                lineStream >> n.x() >> n.y() >> n.z();
                m_normals.push_back((trans * n).normalized());
            }
            else if (prefix == "f")
            {
                std::string vertexStrings[3];
                ObjVertex vertices[3];

                for (int i = 0; i < 3; i++)
                {
                    lineStream >> vertexStrings[i];
                    vertices[i] = ObjVertex(vertexStrings[i]);
                }

                m_faces.push_back({ vertices[0].p, vertices[1].p, vertices[2].p });

                unsigned int indices[3];
                for (int i = 0; i < 3; i++)
                {
                    auto it = vertexMap.find(vertices[i]);
                    if (it == vertexMap.end())
                    {
                        vertexMap[vertices[i]] = static_cast<unsigned int>(m_vertices.size());
                        indices[i] = static_cast<unsigned int>(m_vertices.size());
                        vertexData.push_back(vertices[i]);
                        m_vertices.push_back(std::make_tuple(vertices[i].p, vertices[i].n, vertices[i].uv));
                    }
                    else
                    {
                        indices[i] = it->second;
                    }
                }

                m_triangles.push_back(std::make_tuple(indices[0], indices[1], indices[2]));
            }
        }
    }

    WavefrontObjMesh::~WavefrontObjMesh()
    {
    }

    unsigned int WavefrontObjMesh::submitGeometry(GeometryVisitor* visitor)
    {
        return visitor->addTriangleMesh(this);
    }

    WavefrontObjMesh::ObjVertex::ObjVertex()
    {
    }

    WavefrontObjMesh::ObjVertex::ObjVertex(const std::string& str)
    {
        auto vertexAttribs = Utils::tokenize(str, "/");

        if (vertexAttribs.size() > 0)
        {
            std::stringstream posAttrib(vertexAttribs[0]);
            posAttrib >> p;
            p--;
        }

        if (vertexAttribs.size() > 1 && vertexAttribs[1] != "")
        {
            std::stringstream texCoordAttrib(vertexAttribs[1]);
            texCoordAttrib >> uv;
            uv--;
        }

        if (vertexAttribs.size() > 2 && vertexAttribs[2] != "")
        {
            std::stringstream normalAttrib(vertexAttribs[2]);
            normalAttrib >> n;
            n--;
        }
    }

    bool WavefrontObjMesh::ObjVertex::operator==(const ObjVertex& v) const
    {
        return p == v.p && uv == v.uv && n == v.n;
    }

    std::size_t WavefrontObjMesh::ObjVertexHasher::operator()(const ObjVertex& v) const
    {
        size_t hash = std::hash<int>()(v.p);
        hash = hash * 37 + std::hash<int>()(v.uv);
        hash = hash * 37 + std::hash<int>()(v.n);
        return hash;
    }
}