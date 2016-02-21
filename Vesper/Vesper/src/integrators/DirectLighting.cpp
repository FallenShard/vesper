#include <integrators/DirectLighting.hpp>
#include <core/Scene.hpp>
#include <math/Operations.hpp>
#include <shapes/Intersection.hpp>
#include <bsdfs/BSDF.hpp>
#include <shapes/Shape.hpp>
#include <emitters/Emitter.hpp>

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
            return Spectrum(0.1f);

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

        return bsdf * light * cosFactor;
    }
}