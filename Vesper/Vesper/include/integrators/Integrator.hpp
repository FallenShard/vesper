#pragma once

#include <spectrum/Spectrum.hpp>

namespace vesp
{
    class Scene;
    class Sampler;
    class Ray3f;

    class Integrator
    {
    public:
        Integrator() {};
        virtual ~Integrator() {};

        virtual void preprocess(const Scene* scene) const = 0;

        virtual Spectrum Li(const Scene* scene, const Sampler* sampler, Ray3f& ray) const = 0;

    private:
    };
}