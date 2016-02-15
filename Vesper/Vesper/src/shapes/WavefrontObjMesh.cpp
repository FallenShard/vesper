#include <unordered_map>

#include <acceleration/GeometryVisitor.hpp>

#include <core/Utils.hpp>
#include <shapes/WavefrontObjMesh.hpp>
#include <math/Transform.hpp>

#include <core/Scene.hpp>

namespace
{
    struct ObjVertex
    {
        unsigned int p = -1;
        unsigned int n = -1;
        unsigned int uv = -1;

        inline ObjVertex()
        {
        }

        inline ObjVertex(const std::string& str)
        {
           // std::vector<std::string> tokens = vesp::Utils::tokenize
        }
    };
}

namespace vesp
{
    WavefrontObjMesh::WavefrontObjMesh()
    {
        //using VertexMap = std::unordered_map<ObjVertex, unsigned int, ObjVertexHash>;

        int x;
        x = 3;

        std::ifstream meshFile("res/cube.obj");
        if (meshFile.fail())
        {
            std::cout << "Could not open bunny.obj" << std::endl;
        }

        auto mat = Eigen::Translation<float, 3>(-0.5f, -0.5f, 0.f) * Eigen::DiagonalMatrix<float, 3>(Vector3f(0.5, 0.5, 0.5)) * Eigen::Matrix4f::Identity();

        //auto mat = Eigen::Matrix4f::Identity();

        Transform trans;

        std::cout << "Loading Wavefront Obj mesh : bunny.obj" << std::endl;

        std::vector<Point3f> positions;
        std::vector<Point2f> texCoords;
        std::vector<Normal3f> normals;
        std::vector<Point3i> faces;
        
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
                //p = trans * p;
                positions.push_back({ p.x(), p.y(), p.z() });
            }
            else if (prefix == "vt")
            {
                Point2f tc;
                lineStream >> tc.x() >> tc.y();
                texCoords.push_back(tc);
            }
            else if (prefix == "vn")
            {
                Normal3f n;
                lineStream >> n.x() >> n.y() >> n.z();
                normals.push_back((trans * n).normalized());
            }
            else if (prefix == "f")
            {
                std::string v0, v1, v2;
                lineStream >> v0 >> v1 >> v2;

                int p0, p1, p2;

                p0 = processFaceVertex(v0);
                p1 = processFaceVertex(v1);
                p2 = processFaceVertex(v2);

                faces.push_back({p0 - 1, p1 - 1, p2 - 1});
            }
        }

        if (!faces.empty()) m_faces = faces;
        if (!positions.empty()) m_positions = positions;
        if (!normals.empty()) m_normals = normals;
        if (!texCoords.empty()) m_texCoords = texCoords;
    }

    WavefrontObjMesh::~WavefrontObjMesh()
    {
    }

    unsigned int WavefrontObjMesh::submitGeometry(GeometryVisitor* visitor)
    {
        return visitor->addTriangleMesh(this);
    }

    int WavefrontObjMesh::processFaceVertex(const std::string& faceStr)
    {
        auto vertexAttribs = Utils::tokenize(faceStr, "/");
        
        std::stringstream posAttrib(vertexAttribs[0]);

        int posId;
        posAttrib >> posId;
        return posId;
    }
}