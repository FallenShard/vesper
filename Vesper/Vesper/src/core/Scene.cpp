#include <vector>

#include <core/Scene.hpp>

#include <cameras/Perspective.hpp>

#include <shapes/WavefrontObjMesh.hpp>

namespace vesp
{
    Scene::Scene(int width, int height)
        : m_camera(new PerspectiveCamera(width, height))
    {
        m_shapes.push_back(nullptr); // Account for the plane mesh in wrapper's ctor

        Shape* shape = new WavefrontObjMesh();
        m_shapes.push_back(shape);
        shape->submitGeometry(&m_embreeWrapper);

        m_embreeWrapper.buildAccelStructure();
    }

    Scene::~Scene()
    {
        for (auto& shape : m_shapes)
            delete shape;

        delete m_camera;
    }

    const Camera* Scene::getCamera() const
    {
        return m_camera;
    }

    const Sampler* Scene::getSampler() const
    {
        return nullptr;
    }

    bool Scene::rayIntersect(const Ray3f& ray2, Intersection& its) const
    {
        EmbreeWrapper::HitInfo hit;

        if (m_embreeWrapper.rayIntersect(ray2, hit))
        {
            its.geomId = hit.geomId;
            its.triId = hit.primId;
            return true;
        }

        return false;
    }
}