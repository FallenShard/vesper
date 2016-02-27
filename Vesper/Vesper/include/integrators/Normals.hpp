#pragma once

#include <integrators/Integrator.hpp>

namespace vesp
{
    class NormalsIntegrator : public Integrator
    {
    public:
        NormalsIntegrator(const AttributeList& attributes);
        virtual ~NormalsIntegrator();

        virtual void preprocess(const Scene* scene) override;

        virtual Spectrum Li(const Scene* scene, Sampler& sampler, Ray3f& ray) const override;
    };
}
