#include <emitters/PointLight.hpp>
#include <math/Operations.hpp>

namespace vesp
{
    PointLight::PointLight(const AttributeList& attributes)
    {
        m_position = attributes.getPoint3f("position", Point3f(0.f));
        m_power = attributes.getSpectrum("power", Spectrum(500.f, 500.f, 500.f));
    }

    PointLight::~PointLight()
    {
    }

    Spectrum PointLight::eval(const EmitterSample& emitterSample) const
    {
        return m_power * InvFourPI / (m_position - emitterSample.ref).squaredNorm();
    }

    Spectrum PointLight::sample(EmitterSample& emitterSample, Sampler& sampler) const
    {
        emitterSample.p = m_position;
        emitterSample.pdf = 1.f;
        emitterSample.wi = m_position - emitterSample.ref;

        float squaredDist = emitterSample.wi.squaredNorm();
        emitterSample.wi.normalize();
        emitterSample.shadowRay = Ray3f(emitterSample.ref, emitterSample.wi, RayEpsilon, sqrtf(squaredDist));

        return m_power * InvFourPI / squaredDist;
    }

    float PointLight::pdf(const EmitterSample& emitterSample) const
    {
        return 0.f;
    }

    Spectrum PointLight::samplePhoton(Ray3f& ray, Sampler& sampler) const
    {
        return Spectrum();
    }
}