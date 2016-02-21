#pragma once

#include <integrators/Integrator.hpp>

namespace vesp
{
    class PathTracer : public Integrator
    {
        PathTracer(const AttributeList& attributes);
        virtual ~PathTracer();

        virtual void preprocess(const Scene* scene) override;
        virtual Spectrum Li(const Scene* scene, Sampler& sampler, Ray3f& ray) const override;
    };
}