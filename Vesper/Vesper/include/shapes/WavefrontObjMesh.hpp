#pragma once


#include <tuple>

#include <core/Utils.hpp>
#include <shapes/TriangleMesh.hpp>

namespace vesp
{
    class WavefrontObjMesh : public TriangleMesh
    {
    public:
        WavefrontObjMesh(const AttributeList& attributes);
        virtual ~WavefrontObjMesh();

        virtual unsigned int submitGeometry(GeometryVisitor* visitor) override;

    private:
        struct ObjVertex
        {
            int p = -1;
            int n = -1;
            int uv = -1;

            inline ObjVertex();
            inline ObjVertex(const std::string& str);

            inline bool operator==(const ObjVertex& v) const;
        };

        struct ObjVertexHasher
        {
            std::size_t operator()(const ObjVertex& v) const;
        };
    };
}