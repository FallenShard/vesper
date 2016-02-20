#pragma once

#include <spectrum/Spectrum.hpp>
#include <math/Types.hpp>
#include <core/VesperObject.hpp>

namespace vesp
{
    class Scene;
    class Sampler;

    class Integrator : public VesperObject
    {
    public:
        virtual ~Integrator() {};

        virtual void preprocess(const Scene* scene) = 0;

        virtual Spectrum Li(const Scene* scene, const Sampler* sampler, Ray3f& ray) const = 0;

        virtual ClassType getClassType() const override { return ClassType::Integrator; }
    };
}