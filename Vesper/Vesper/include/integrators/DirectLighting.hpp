#pragma once

#include <integrators/Integrator.hpp>

namespace vesp
{
    class DirectLightingIntegrator : public Integrator
    {
    public:
        DirectLightingIntegrator(const AttributeList& attributes);
        virtual ~DirectLightingIntegrator();

        virtual void preprocess(const Scene* scene) override;

        virtual Spectrum Li(const Scene* scene, Sampler& sampler, Ray3f& ray) const override;

    private:
        Spectrum emitterImportanceSample(const Scene* scene, Sampler& sampler, const Ray3f& ray, const Intersection& its) const;
        Spectrum bsdfImportanceSample(const Scene* scene, Sampler& sampler, const Ray3f& ray, const Intersection& its) const;
    };
}
