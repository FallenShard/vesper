#include <bsdfs/Dielectric.hpp>
#include <optics/Fresnel.hpp>
#include <math/Frame.hpp>
#include <sampling/Sampler.hpp>

namespace vesp
{
    DielectricBSDF::DielectricBSDF(const AttributeList& attributes)
    {
        m_intIOR = attributes.getFloat("intIOR", 1.5046f);
        m_extIOR = attributes.getFloat("extIOR", 1.000277f);
    }

    DielectricBSDF::~DielectricBSDF()
    {
    }

    Spectrum DielectricBSDF::eval(const BSDFSample& bsdfSample) const
    {
        return Spectrum(0.f);
    }

    Spectrum DielectricBSDF::sample(BSDFSample& bsdfSample, Sampler& sampler) const
    {
        // Get the fresnel coefficient
        float cosThetaI = Frame::cosTheta(bsdfSample.wi);
        float reflCoeff = Fresnel::dielectric(cosThetaI, m_extIOR, m_intIOR);

        // Set the measure to discrete
        bsdfSample.measure = Measure::Discrete;
        bsdfSample.interactionType = BSDFType::Delta;

        float sample = sampler.next1D();

        // If sample is less than fresnelCoeff, reflect
        if (sample <= reflCoeff) // Reflect
        {
            // Reflection in local coordinates
            bsdfSample.wo = Vector3f(-bsdfSample.wi.x(), -bsdfSample.wi.y(), bsdfSample.wi.z());
            bsdfSample.eta = 1.f;

            return Spectrum(1.f);
        }

        // otherwise, refract
        // These may be swapped if we come "from the inside"
        float etaExt = m_extIOR, etaInt = m_intIOR;

        // Normal to use for refraction direction formula
        Normal3f n(0.f, 0.f, 1.f);
        Vector3f wi = bsdfSample.wi;

        // If the angle was negative, we're coming from the inside, update relevant variables
        if (cosThetaI < 0.f)
        {
            std::swap(etaExt, etaInt);
            cosThetaI = -cosThetaI;
            n = -n;
        }

        // Set eta ratio
        float etaRatio = etaExt / etaInt;
        bsdfSample.eta = etaRatio;

        // Set outgoing direction
        float sinThetaTSqr = etaRatio * etaRatio * (1.f - cosThetaI * cosThetaI);
        bsdfSample.wo = -etaRatio * (wi - cosThetaI * n) - n * sqrtf(1.f - sinThetaTSqr);

        // Return the bsdf sample
        return Spectrum(etaRatio * etaRatio);
    }

    float DielectricBSDF::pdf(const BSDFSample& bsdfSample) const
    {
        return 0.f;
    }

    unsigned int DielectricBSDF::getType() const
    {
        return BSDFType::Delta;
    }
}