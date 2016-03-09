#include <integrators/MisPathTracer.hpp>
#include <sampling/Sampler.hpp>

namespace vesp
{
    inline float miWeight(float pdf1, float pdf2)
    {
        pdf1 *= pdf1;
        pdf2 *= pdf2;
        return pdf1 / (pdf1 + pdf2);
    }

    MisPathTracer::MisPathTracer(const AttributeList& attribs)
    {
    }

    MisPathTracer::~MisPathTracer()
    {
    }

    void MisPathTracer::preprocess(const Scene* scene)
    {
    }

    Spectrum MisPathTracer::Li(const Scene* scene, Sampler& sampler, Ray3f& ray) const
    {
        Spectrum L(0.f);

        Intersection its;

        Spectrum throughput(1.f);
        bool isSpecular = true;

        while (true)
        {
            if (!scene->rayIntersect(ray, its))
                break;

            if (isSpecular && its.shape->getEmitter())
            {
                EmitterSample emittanceSample(ray.o, its.p, its.shFrame.n);
                L += throughput * its.shape->getEmitter()->eval(emittanceSample);
            }

            if (its.shape->getBSDF()->getType() != BSDFType::Delta)
            {
                EmitterSample emSam(its.p);
                auto lightSpec = scene->sampleEmitter(its, sampler, emSam);

                float cosFactor = its.shFrame.n.dot(emSam.wi);
                if (!(cosFactor <= 0.f || lightSpec.isZero() || scene->rayIntersect(emSam.shadowRay)))
                {
                    BSDFSample bsdfSam(its.p, its.toLocal(-ray.d), its.toLocal(emSam.wi));
                    bsdfSam.measure = Measure::SolidAngle;
                    auto bsdfSpec = its.shape->getBSDF()->eval(bsdfSam);

                    float pdfEm = emSam.pdf;
                    float pdfBsdf = its.shape->getBSDF()->pdf(bsdfSam);
                    L += throughput * bsdfSpec * lightSpec * cosFactor * miWeight(pdfEm, pdfBsdf);
                }
            }

            BSDFSample bsdfSample(its.p, its.toLocal(-ray.d));
            auto bsdf = its.shape->getBSDF()->sample(bsdfSample, sampler);
            Intersection bsdfIts;
            Ray3f bsdfRay(its.p, its.toWorld(bsdfSample.wo));
            if (scene->rayIntersect(bsdfRay, bsdfIts) && bsdfIts.shape->getEmitter())
            {
                const auto* em = bsdfIts.shape->getEmitter();
                
                EmitterSample emSam(its.p, bsdfIts.p, bsdfIts.shFrame.n);
                emSam.wi = bsdfRay.d;
                auto lightSpec = em->eval(emSam);

                float pdfBsdf = its.shape->getBSDF()->pdf(bsdfSample);
                float pdfEm = em->pdf(emSam) * scene->getEmitterPdf();
                if (pdfBsdf + pdfEm > 0.f)
                    L += throughput * bsdf * lightSpec * miWeight(pdfBsdf, pdfEm);
            }

            isSpecular = its.shape->getBSDF()->getType() == BSDFType::Delta;
            throughput *= bsdf;
            ray = bsdfRay;

            float q = 1.f - std::min(throughput.maxCoeff(), 0.99f);
            if (sampler.next1D() > q)
                throughput /= (1.f - q);
            else
                break;
        }

        return L;
    }
}