#pragma once

#include <embree2/rtcore.h>
#include <embree2/rtcore_ray.h>

#include <math/Types.hpp>

#include <acceleration/GeometryVisitor.hpp>

namespace vesp
{
    class Shape;

    class EmbreeWrapper : public GeometryVisitor
    {
    public:
        struct HitInfo
        {
            unsigned int geomId;
            unsigned int primId;
        };

        EmbreeWrapper();
        ~EmbreeWrapper();

        void buildAccelStructure();

        bool rayIntersect(const Ray3f& ray, HitInfo& hitInfo) const;

        virtual unsigned int addTriangleMesh(TriangleMesh* mesh) override;


    private:
        struct Vertex
        {
            float x, y, z, w;
        };

        struct Face
        {
            unsigned int v0, v1, v2;
        };

        RTCDevice m_device;
        RTCScene m_scene;

        RTCRay m_currentRay; // Avoid constant re-allocation of the ray
    };
}