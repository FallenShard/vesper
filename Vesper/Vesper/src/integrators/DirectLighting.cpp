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

        L += emitterImportanceSample(scene, sampler, ray, its);
        L += bsdfImportanceSample(scene, sampler, ray, its);

        return L;
    }
    Spectrum DirectLightingIntegrator::emitterImportanceSample(const Scene* scene, Sampler& sampler, const Ray3f& ray, const Intersection& its) const
    {
        Spectrum Li(0.f);

        EmitterSample emSam(its.p);
        auto light = scene->sampleEmitter(its, sampler, emSam);

        float cosFactor = its.shFrame.n.dot(emSam.wi);
        if (cosFactor <= 0.f || light.isZero() || scene->rayIntersect(emSam.shadowRay))
            return Li;

        BSDFSample bsdfSam(its.p, its.toLocal(-ray.d), its.toLocal(emSam.wi));
        bsdfSam.measure = Measure::SolidAngle;
        auto bsdf = its.shape->getBSDF()->eval(bsdfSam);
        Li = bsdf * light * cosFactor;

        float pdfEm = emSam.pdf;
        float pdfBsdf = its.shape->getBSDF()->pdf(bsdfSam);
        return pdfEm * Li / (pdfEm + pdfBsdf);
    }
    Spectrum DirectLightingIntegrator::bsdfImportanceSample(const Scene* scene, Sampler& sampler, const Ray3f& ray, const Intersection& its) const
    {
        Spectrum Li(0.f);

        BSDFSample bsdfSam(its.p, its.toLocal(-ray.d));
        bsdfSam.measure = Measure::SolidAngle;
        auto bsdf = its.shape->getBSDF()->sample(bsdfSam, sampler);

        Intersection bsdfIts;
        Ray3f bsdfRay(its.p, its.toWorld(bsdfSam.wo));
        if (!scene->rayIntersect(bsdfRay, bsdfIts))
            return Li;

        if (!bsdfIts.shape->getEmitter())
            return Li;

        const auto* em = bsdfIts.shape->getEmitter();

        EmitterSample emSam(its.p, bsdfIts.p, bsdfIts.shFrame.n);
        emSam.wi = bsdfRay.d;
        auto light = em->eval(emSam);
        Li = bsdf * light;

        float pdfBsdf = its.shape->getBSDF()->pdf(bsdfSam);
        float pdfEm = em->pdf(emSam) * scene->getEmitterPdf();
        return pdfBsdf * Li / (pdfBsdf + pdfEm);
    }
}