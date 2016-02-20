#pragma once

#include <vector>

#include <math/Types.hpp>

namespace vesp
{
    class VesperObject;
    class Sampler;
    class Integrator;
    class Sensor;
    class Shape;
    struct Intersection;
    class EmbreeWrapper;

    class Scene
    {
    public:
        Scene();
        ~Scene();

        void addChild(std::shared_ptr<VesperObject> child);
        void configure();
        
        const Sampler* getSampler() const;
        const Integrator* getIntegrator() const;
        const Sensor* getSensor() const;

        bool rayIntersect(const Ray3f& ray, Intersection& its) const;
        bool rayIntersect(const Ray3f& shadowRay) const;
        
    private:
        std::shared_ptr<EmbreeWrapper> m_embreeWrapper;

        std::shared_ptr<Sampler> m_sampler;

        std::vector<std::shared_ptr<Shape>> m_shapes;

        std::shared_ptr<Sensor> m_sensor;

        std::shared_ptr<Integrator> m_integrator;

        Vector2i m_imageSize;
    };
}