#include <math/Operations.hpp>
#include <bsdfs/Lambertian.hpp>
#include <math/Frame.hpp>
#include <sampling/Sampler.hpp>
#include <sampling/Warp.hpp>

namespace vesp
{
    LambertianBSDF::LambertianBSDF(const AttributeList& attributes)
    {
        m_reflectance = attributes.getSpectrum("reflectance", Spectrum(1.f));
    }

    LambertianBSDF::~LambertianBSDF()
    {
    }

    Spectrum LambertianBSDF::eval(const BSDFSample& bsdfSample) const
    {
        if (bsdfSample.measure != Measure::SolidAngle ||
            Frame::cosTheta(bsdfSample.wi) <= 0.f ||
            Frame::cosTheta(bsdfSample.wo) <= 0.f)
            return 0.f;

        return m_reflectance * InvPI;
    }

    Spectrum LambertianBSDF::sample(BSDFSample& bsdfSample, Sampler& sampler) const
    {
        bsdfSample.measure = Measure::SolidAngle;
        bsdfSample.interactionType = BSDFType::Diffuse;
        bsdfSample.eta = 1.f;
        bsdfSample.wo = Warp::squareToCosineHemisphere(sampler.next2D());
        // eval() / pdf() = albedo * invPI / (cosThetaI * invPI) * cosThetaI(subtension)
        // account for cosine subtension = just albedo :)
        return m_reflectance;
    }

    float LambertianBSDF::pdf(const BSDFSample& bsdfSample) const
    {
        if (bsdfSample.measure != Measure::SolidAngle ||
            Frame::cosTheta(bsdfSample.wi) <= 0.f ||
            Frame::cosTheta(bsdfSample.wo) <= 0.f)
            return 0.f;

        return InvPI * Frame::cosTheta(bsdfSample.wo);
    }

    unsigned int LambertianBSDF::getType() const
    {
        return BSDFType::Diffuse;
    }
}