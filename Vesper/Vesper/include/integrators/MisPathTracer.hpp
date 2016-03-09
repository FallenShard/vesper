#pragma once

#include <integrators/Integrator.hpp>

namespace vesp
{
    class MisPathTracer : public Integrator
    {
    public:
        MisPathTracer(const AttributeList& attributes);
        virtual ~MisPathTracer();

        virtual void preprocess(const Scene* scene) override;
        virtual Spectrum Li(const Scene* scene, Sampler& sampler, Ray3f& ray) const override;
    };
}