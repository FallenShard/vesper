#pragma once

#include <vector>

#include <math/Types.hpp>
#include <spectrum/Spectrum.hpp>

namespace vesp
{
    class VesperObject;
    class Sampler;
    class Integrator;
    class Sensor;
    class Emitter;
    class Shape;
    
    class EmbreeWrapper;

    struct Intersection;
    struct EmitterSample;

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

        const Emitter* getRandomEmitter(float sample) const;
        float getEmitterPdf() const;
        Spectrum sampleEmitter(const Intersection& its, Sampler& sampler, EmitterSample& emitterSample) const;

        bool rayIntersect(const Ray3f& ray, Intersection& its) const;
        bool rayIntersect(const Ray3f& shadowRay) const;
        
    private:
        std::shared_ptr<EmbreeWrapper> m_embreeWrapper;

        std::shared_ptr<Sampler> m_sampler;
        std::shared_ptr<Integrator> m_integrator;

        std::vector<std::shared_ptr<Emitter>> m_emitters;
        std::vector<std::shared_ptr<Shape>> m_shapes;

        std::shared_ptr<Sensor> m_sensor;

        

        Vector2i m_imageSize;
    };
}