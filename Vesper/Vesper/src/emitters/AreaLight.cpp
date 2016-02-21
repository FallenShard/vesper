#include <emitters/AreaLight.hpp>
#include <shapes/Shape.hpp>
#include <sampling/Sampler.hpp>
#include <sampling/Warp.hpp>

namespace vesp
{
    AreaLight::AreaLight(const AttributeList& attributes)
    {
        m_radiance = attributes.getSpectrum("radiance");
    }

    Spectrum AreaLight::eval(const EmitterSample& emitterSample) const
    {
        Vector3f wi = emitterSample.p - emitterSample.ref;
        float cosTheta = emitterSample.n.dot((-wi).normalized());
        if (cosTheta <= 0.f)
            return Spectrum(0.f);

        return m_radiance;
    }

    Spectrum AreaLight::sample(EmitterSample& emitterSample, Sampler& sampler) const
    {
        ShapeSample shapeSample(emitterSample.ref);
        m_shape->sampleSurface(shapeSample, sampler);

        emitterSample.p = shapeSample.p;
        emitterSample.n = shapeSample.n;
        emitterSample.wi = emitterSample.p - emitterSample.ref;

        float squaredDist = emitterSample.wi.squaredNorm();
        emitterSample.wi.normalize();

        float cosTheta = emitterSample.n.dot(-emitterSample.wi);

        emitterSample.shadowRay = Ray3f(emitterSample.ref, emitterSample.wi, RayEpsilon, sqrt(squaredDist) - RayEpsilon);

        Spectrum radiance(0.f);

        if (cosTheta <= 0.f)
            emitterSample.pdf = 0.f;
        else
        {
            emitterSample.pdf = shapeSample.pdf * squaredDist / cosTheta; // Jacobian term
            radiance = m_radiance / emitterSample.pdf;
        }

        return radiance;
    }

    float AreaLight::pdf(const EmitterSample& emitterSample) const
    {
        ShapeSample shapeSample(emitterSample.ref);
        float areaPdf = m_shape->pdfSurface(shapeSample);
        float cosTheta = emitterSample.n.dot(-emitterSample.wi);
        if (cosTheta <= 0.f)
            return 0.f;

        float squaredDist = (emitterSample.p - emitterSample.ref).squaredNorm();
        return areaPdf * squaredDist / cosTheta;
    }

    Spectrum AreaLight::samplePhoton(Ray3f& ray, Sampler& sampler) const
    {
        ShapeSample shapeSample;
        m_shape->sampleSurface(shapeSample, sampler);

        Vector3f dir = Warp::squareToCosineHemisphere(sampler.next2D());
        Frame frame(shapeSample.n);

        ray = Ray3f(shapeSample.p, frame.toWorld(dir));
        return PI * m_radiance / m_shape->pdfSurface(shapeSample);
    }
}