#include <integrators/AmbientOcclusion.hpp>
#include <shapes/Intersection.hpp>
#include <core/Scene.hpp>
#include <sampling/Sampler.hpp>
#include <sampling/Warp.hpp>

namespace vesp
{
    AmbientOcclusionIntegrator::AmbientOcclusionIntegrator(const AttributeList& attribs)
    {
        m_length = attribs.getFloat("length", 1.f);
    }

    void AmbientOcclusionIntegrator::preprocess(const Scene* scene)
    {
    }

    Spectrum AmbientOcclusionIntegrator::Li(const Scene* scene, Sampler& sampler, Ray3f& ray) const
    {
        Intersection its;
        if (!scene->rayIntersect(ray, its))
            return Spectrum(1.f);

        Vector3f worldDir = its.toWorld(Warp::squareToUniformHemisphere(sampler.next2D()));
        Ray3f occlusionRay(its.p, worldDir, RayEpsilon, m_length);

        if (scene->rayIntersect(occlusionRay))
            return Spectrum(0.f);
        else
            return Spectrum(1.f);
    }
}