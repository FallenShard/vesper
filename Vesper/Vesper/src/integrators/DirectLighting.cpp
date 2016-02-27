#include <integrators/DirectLighting.hpp>

namespace vesp
{
    DirectLightingIntegrator::DirectLightingIntegrator(const AttributeList& attributes)
    {
    }
    
    DirectLightingIntegrator::~DirectLightingIntegrator()
    {
    }

    void DirectLightingIntegrator::preprocess(const Scene* scene)
    {
    }
    
    Spectrum DirectLightingIntegrator::Li(const Scene* scene, Sampler& sampler, Ray3f& ray) const
    {
        Intersection its;
        if (!scene->rayIntersect(ray, its))
            return Spectrum(0.0f);

        Spectrum L(0.f);

        if (its.shape->getEmitter())
        {
            EmitterSample emittanceSample(ray.o, its.p, its.shFrame.n);
            L += its.shape->getEmitter()->eval(emittanceSample);
        }

        EmitterSample emSam(its.p);
        auto light = scene->sampleEmitter(its, sampler, emSam);

        float cosFactor = its.shFrame.n.dot(emSam.wi);
        if (cosFactor <= 0.f || light.isZero() || scene->rayIntersect(emSam.shadowRay))
            return L;
        
        BSDFSample bsdfSam(its.p, its.toLocal(-ray.d), its.toLocal(emSam.wi));
        bsdfSam.measure = Measure::SolidAngle;
        auto bsdf = its.shape->getBSDF()->eval(bsdfSam);

        return L + bsdf * light * cosFactor;
    }
}