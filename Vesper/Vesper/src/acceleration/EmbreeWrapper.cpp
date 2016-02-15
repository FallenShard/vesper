#include <acceleration/EmbreeWrapper.hpp>

namespace vesp
{
    EmbreeWrapper::EmbreeWrapper()
        : m_device(rtcNewDevice(nullptr))
        , m_scene(nullptr)
    {
        m_scene = rtcDeviceNewScene(m_device, RTC_SCENE_STATIC, RTC_INTERSECT1);

        std::vector<Vertex> verts;
        verts.push_back({ -3.f, -2.f, +15.f, 1.f });
        verts.push_back({ +3.f, -2.f, +15.f, 1.f });
        verts.push_back({ +3.f, -2.f, +25.f, 1.f });
        verts.push_back({ -3.f, -2.f, +25.f, 1.f });

        std::vector<Face> triangles;
        triangles.push_back({ 0, 1, 2 });
        triangles.push_back({ 0, 2, 3 });

        unsigned int geomId = rtcNewTriangleMesh(m_scene, RTC_GEOMETRY_STATIC, 2, 4, 1);

        Vertex* vertices = (Vertex*)rtcMapBuffer(m_scene, geomId, RTC_VERTEX_BUFFER);
        for (int i = 0; i < 4; i++)
            vertices[i] = verts[i];
        rtcUnmapBuffer(m_scene, geomId, RTC_VERTEX_BUFFER);

        Face* faces = (Face*)rtcMapBuffer(m_scene, geomId, RTC_INDEX_BUFFER);
        for (int i = 0; i < 2; i++)
            faces[i] = triangles[i];
        rtcUnmapBuffer(m_scene, geomId, RTC_INDEX_BUFFER);
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
        RTCRay m_currentRay;

        for (size_t i = 0; i < 3; i++)
        {
            m_currentRay.org[i] = ray.o.coeff(i);
            m_currentRay.dir[i] = ray.d.coeff(i);
        }
        m_currentRay.tnear = ray.minT;
        m_currentRay.tfar  = ray.maxT;
        m_currentRay.geomID = RTC_INVALID_GEOMETRY_ID;
        m_currentRay.primID = RTC_INVALID_GEOMETRY_ID;
        m_currentRay.instID = RTC_INVALID_GEOMETRY_ID;
        m_currentRay.mask = 0xFFFFFFFF;
        m_currentRay.time = 0.f;

        rtcIntersect(m_scene, m_currentRay);

        if (m_currentRay.geomID != RTC_INVALID_GEOMETRY_ID)
        {
            hitInfo.primId = m_currentRay.primID;
            hitInfo.geomId = m_currentRay.geomID;
            return true;
        }

        return false;
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