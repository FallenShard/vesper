#pragma once

#include <spectrum/Spectrum.hpp>
#include <math/Types.hpp>
#include <core/VesperObject.hpp>
#include <core/Scene.hpp>
#include <math/Operations.hpp>
#include <shapes/Intersection.hpp>
#include <bsdfs/BSDF.hpp>
#include <shapes/Shape.hpp>
#include <emitters/Emitter.hpp>

namespace vesp
{
    class Scene;
    class Sampler;

    class Integrator : public VesperObject
    {
    public:
        virtual ~Integrator() {};

        virtual void preprocess(const Scene* scene) = 0;

        virtual Spectrum Li(const Scene* scene, Sampler& sampler, Ray3f& ray) const = 0;

        virtual ClassType getClassType() const override { return ClassType::Integrator; }
    };
}