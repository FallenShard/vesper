#include <integrators/PathTracer.hpp>

namespace vesp
{
    PathTracer::PathTracer(const AttributeList& attribs)
    {
    }

    PathTracer::~PathTracer()
    {
    }

    void PathTracer::preprocess(const Scene* scene)
    {
    }

    Spectrum PathTracer::Li(const Scene* scene, Sampler& sampler, Ray3f& ray) const
    {
        Spectrum color(0.f);

        Intersection its;
        
        while (true)
        {
            if (scene->rayIntersect(ray, its))
                return Spectrum(0.f);
        }
    }
}