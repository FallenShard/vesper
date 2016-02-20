#pragma once

#include <integrators/Integrator.hpp>

namespace vesp
{
    class TestIntegrator : public Integrator
    {
    public:
        TestIntegrator(const AttributeList& attributes);
        virtual ~TestIntegrator();

        virtual void preprocess(const Scene* scene) override;

        virtual Spectrum Li(const Scene* scene, const Sampler* sampler, Ray3f& ray) const override;
    };
}
