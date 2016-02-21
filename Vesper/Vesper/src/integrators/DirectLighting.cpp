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

        EmitterSample emSam;
        emSam.ref = its.p;
        auto light = scene->sampleEmitter(its, sampler, emSam);

        float cosFactor = its.shFrame.n.dot(emSam.wi);
        if (cosFactor < 0.f || scene->rayIntersect(emSam.shadowRay))
            return Spectrum(0.f);
        
        BSDFSample bsdfSam;
        bsdfSam.p = its.p;
        bsdfSam.wo = its.toLocal(-ray.d);
        auto bsdf = its.shape->getBSDF()->eval(bsdfSam);

        Spectrum color = bsdf * light * cosFactor;
        if (its.shape->getEmitter())
            color += its.shape->getEmitter()->eval(emSam);

        return color;
    }
}