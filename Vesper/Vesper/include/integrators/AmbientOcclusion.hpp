#pragma once

#include <integrators/Integrator.hpp>

namespace vesp
{
    class AmbientOcclusionIntegrator : public Integrator
    {
    public:
        AmbientOcclusionIntegrator(const AttributeList& attributes);

        virtual void preprocess(const Scene* scene) override;

        virtual Spectrum Li(const Scene* scene, Sampler& sampler, Ray3f& ray) const override;

    private:
        float m_length;
    };
}
