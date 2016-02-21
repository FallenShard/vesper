#pragma once

#include <emitters/Emitter.hpp>

namespace vesp
{
    class AreaLight : public Emitter
    {
    public:
        AreaLight(const AttributeList& attributes);

        virtual Spectrum eval(const EmitterSample& emitterSample) const override;
        virtual Spectrum sample(EmitterSample& emitterSample, Sampler& sampler) const override;
        virtual float pdf(const EmitterSample& emitterSample) const override;

        virtual Spectrum samplePhoton(Ray3f& ray, Sampler& sampler) const override;

    private:
        Spectrum m_radiance;
    };
}