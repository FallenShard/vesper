#include <integrators/Normals.hpp>
#include <iostream>

namespace vesp
{
    NormalsIntegrator::NormalsIntegrator(const AttributeList& attributes)
    {
    }

    NormalsIntegrator::~NormalsIntegrator()
    {
    }

    void NormalsIntegrator::preprocess(const Scene* scene)
    {
    }

    Spectrum NormalsIntegrator::Li(const Scene* scene, Sampler& sampler, Ray3f& ray) const
    {
        Intersection its;
        if (!scene->rayIntersect(ray, its))
            return Spectrum(0.0f);

        Spectrum color;
        color.r() = std::fabsf(its.shFrame.n.x());
        color.g() = std::fabsf(its.shFrame.n.y());
        color.b() = std::fabsf(its.shFrame.n.z());

        return color;
    }
}