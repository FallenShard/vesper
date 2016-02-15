#pragma once

#include <shapes/TriangleMesh.hpp>

namespace vesp
{
    class WavefrontObjMesh : public TriangleMesh
    {
    public:
        WavefrontObjMesh();
        virtual ~WavefrontObjMesh();

        virtual unsigned int submitGeometry(GeometryVisitor* visitor) override;

    private:
        int processFaceVertex(const std::string& faceDescriptor);
    };
}