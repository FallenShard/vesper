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
        Spectrum L(0.f);

        Intersection its;

        Spectrum throughput(1.f);
        
        while (true)
        {
            if (!scene->rayIntersect(ray, its))
                return L;

            if (its.shape->getEmitter())
            {
                EmitterSample emittanceSample(ray.o, its.p, its.shFrame.n);
                L += throughput * its.shape->getEmitter()->eval(emittanceSample);
            }

            BSDFSample bsdfSample(its.p, its.toLocal(-ray.d));
            throughput *= its.shape->getBSDF()->sample(bsdfSample, sampler);

            ray.o = its.p;
            ray.d = its.toWorld(bsdfSample.wo);
            ray.update();
        }
    }
}