#include <acceleration/EmbreeWrapper.hpp>

namespace vesp
{
    EmbreeWrapper::EmbreeWrapper()
        : m_device(rtcNewDevice(nullptr))
        , m_scene(nullptr)
    {
        m_scene = rtcDeviceNewScene(m_device, RTC_SCENE_STATIC, RTC_INTERSECT1);
    }

    EmbreeWrapper::~EmbreeWrapper()
    {
        rtcDeleteScene(m_scene);
        rtcDeleteDevice(m_device);
    }

    void EmbreeWrapper::buildAccelStructure()
    {
        rtcCommit(m_scene);
    }

    bool EmbreeWrapper::rayIntersect(const Ray3f& ray, HitInfo& hitInfo) const
    {
        RTCRay rtcRay;

        for (size_t i = 0; i < 3; i++)
        {
            rtcRay.org[i] = ray.o.coeff(i);
            rtcRay.dir[i] = ray.d.coeff(i);
        }
        rtcRay.tnear = ray.minT;
        rtcRay.tfar  = ray.maxT;
        rtcRay.geomID = RTC_INVALID_GEOMETRY_ID;
        rtcRay.primID = RTC_INVALID_GEOMETRY_ID;
        rtcRay.instID = RTC_INVALID_GEOMETRY_ID;
        rtcRay.mask = 0xFFFFFFFF;
        rtcRay.time = ray.time;

        rtcIntersect(m_scene, rtcRay);

        if (rtcRay.geomID != RTC_INVALID_GEOMETRY_ID)
        {
            hitInfo.tHit = rtcRay.tfar;
            hitInfo.geoN = Vector3f(rtcRay.Ng[0], rtcRay.Ng[1], rtcRay.Ng[2]).normalized();
            hitInfo.u = rtcRay.u;
            hitInfo.v = rtcRay.v;
            hitInfo.primId = rtcRay.primID;
            hitInfo.geomId = rtcRay.geomID;
            hitInfo.instId = rtcRay.instID;
            return true;
        }

        return false;
    }

    bool EmbreeWrapper::rayIntersect(const Ray3f& shadowRay) const
    {
        RTCRay rtcRay;

        for (size_t i = 0; i < 3; i++)
        {
            rtcRay.org[i] = shadowRay.o.coeff(i);
            rtcRay.dir[i] = shadowRay.d.coeff(i);
        }
        rtcRay.tnear = shadowRay.minT;
        rtcRay.tfar = shadowRay.maxT;
        rtcRay.geomID = RTC_INVALID_GEOMETRY_ID;
        rtcRay.primID = RTC_INVALID_GEOMETRY_ID;
        rtcRay.instID = RTC_INVALID_GEOMETRY_ID;
        rtcRay.mask = 0xFFFFFFFF;
        rtcRay.time = shadowRay.time;

        rtcOccluded(m_scene, rtcRay);
        return rtcRay.geomID == 0;
    }

    unsigned int EmbreeWrapper::addTriangleMesh(TriangleMesh* mesh)
    {
        unsigned int geomId = rtcNewTriangleMesh(m_scene, RTC_GEOMETRY_STATIC, mesh->getNumTriangles(), mesh->getNumVertices(), 1);

        Vertex* vertices = static_cast<Vertex*>(rtcMapBuffer(m_scene, geomId, RTC_VERTEX_BUFFER));
        auto& positions = mesh->getVertexPositions();
        for (int i = 0; i < positions.size(); i++)
        {
            vertices[i].x = positions[i].x();
            vertices[i].y = positions[i].y();
            vertices[i].z = positions[i].z();
            vertices[i].w = 1.f;
        }
        rtcUnmapBuffer(m_scene, geomId, RTC_VERTEX_BUFFER);

        Face* faces = static_cast<Face*>(rtcMapBuffer(m_scene, geomId, RTC_INDEX_BUFFER));
        auto& triangles = mesh->getTriangleIndices();
        for (int i = 0; i < mesh->getTriangleIndices().size(); i++)
        {
            faces[i].v0 = triangles[i].x();
            faces[i].v1 = triangles[i].y();
            faces[i].v2 = triangles[i].z();
        }
        rtcUnmapBuffer(m_scene, geomId, RTC_INDEX_BUFFER);
        
        return geomId;
    }
}