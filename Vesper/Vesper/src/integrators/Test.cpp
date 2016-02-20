#include <integrators/Test.hpp>
#include <core/Scene.hpp>
#include <math/Operations.hpp>
#include <shapes/Intersection.hpp>
#include <bsdfs/BSDF.hpp>
#include <shapes/Shape.hpp>

#include <iostream>

namespace vesp
{
    TestIntegrator::TestIntegrator(const AttributeList& attributes)
    {
    }
    
    TestIntegrator::~TestIntegrator()
    {
    }

    void TestIntegrator::preprocess(const Scene* scene)
    {
    }
    
    Spectrum TestIntegrator::Li(const Scene* scene, const Sampler* sampler, Ray3f& ray) const
    {
        Intersection its;
        if (!scene->rayIntersect(ray, its))
            return Spectrum(0.1f);

        Spectrum color;

        Point3f ls(-2.f, 2.f, 3.f); // fixed point light, for now :)
        Spectrum l(150.f);

        Vector3f wi = ls - its.p;

        float dist2 = wi.squaredNorm();
        wi.normalize();

        float cosFactor = its.shFrame.n.dot(wi);
        Ray3f shadowRay(its.p, wi, RayEpsilon, sqrt(dist2));
        bool isOccluded = scene->rayIntersect(shadowRay);

        if (isOccluded || cosFactor < 0.f)
            return Spectrum(0.f);

        Spectrum brdf = its.shape->getBSDF()->eval(BSDFSample());

        color = brdf * (l * InvFourPI / dist2) * its.shFrame.n.dot(wi);
        
        return color;
    }
}