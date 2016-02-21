#pragma once

#include <emitters/Emitter.hpp>

namespace vesp
{
    class PointLight : public Emitter
    {
    public:
        PointLight(const AttributeList& attributes);
        ~PointLight();

        virtual Spectrum eval(const EmitterSample& emitterSample) const override;
        virtual Spectrum sample(EmitterSample& emitterSample, Sampler& sampler) const override;
        virtual float pdf(const EmitterSample& emitterSample) const override;

        virtual Spectrum samplePhoton(Ray3f& ray, const Sampler& sampler) const override;

    private:
        Point3f m_position;
        Spectrum m_power;
    };
}