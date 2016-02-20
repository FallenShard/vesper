#include <memory>

#include <core/Scene.hpp>
#include <core/VesperObject.hpp>
#include <acceleration/EmbreeWrapper.hpp>
#include <sampling/Sampler.hpp>
#include <integrators/Integrator.hpp>
#include <sensors/Sensor.hpp>
#include <shapes/Shape.hpp>

namespace vesp
{
    Scene::Scene()
        : m_integrator(nullptr)
        , m_sensor(nullptr)
        , m_sampler(nullptr)
        , m_embreeWrapper(nullptr)
    {
    }

    Scene::~Scene()
    {
    }

    void Scene::addChild(std::shared_ptr<VesperObject> child)
    {
        switch (child->getClassType())
        {
        case VesperObject::ClassType::Integrator:
        {
            m_integrator = std::static_pointer_cast<Integrator>(child);
            break;
        }
        
        case VesperObject::ClassType::Sampler:
        {
            m_sampler = std::static_pointer_cast<Sampler>(child);
            break;
        }

        case VesperObject::ClassType::Sensor:
        {
            m_sensor = std::static_pointer_cast<Sensor>(child);
            break;
        }

        case VesperObject::ClassType::Shape:
        {
            m_shapes.emplace_back(std::static_pointer_cast<Shape>(child));
            break;
        }
        }
    }

    void Scene::configure()
    {
        m_imageSize = m_sensor->getImageSize();


        m_embreeWrapper = std::make_shared<EmbreeWrapper>();
        for (auto& shape : m_shapes)
            shape->submitGeometry(m_embreeWrapper.get());
        m_embreeWrapper->buildAccelStructure();
    }

    const Sensor* Scene::getSensor() const
    {
        return m_sensor.get();
    }

    const Sampler* Scene::getSampler() const
    {
        return m_sampler.get();
    }

    const Integrator* Scene::getIntegrator() const
    {
        return m_integrator.get();
    }

    bool Scene::rayIntersect(const Ray3f& ray, Intersection& its) const
    {
        EmbreeWrapper::HitInfo hit;

        if (m_embreeWrapper->rayIntersect(ray, hit))
        {
            its.uv = Point2f(hit.u, hit.v); // makes sense for meshes only; u, v are barycentric coords

            m_shapes[hit.geomId]->setIntersectionInfo(hit.primId, ray, its);

            return true;
        }

        return false;
    }

    bool Scene::rayIntersect(const Ray3f& shadowRay) const
    {
        return m_embreeWrapper->rayIntersect(shadowRay);
    }
}