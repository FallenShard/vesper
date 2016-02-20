#include <math/Operations.hpp>
#include <bsdfs/Lambertian.hpp>

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
        return m_reflectance * InvPI;
    }

    Spectrum LambertianBSDF::sample(BSDFSample& bsdfSample, const Sampler& sampler) const
    {
        // eval() / pdf() = albedo * invPI / (cosThetaI * invPI) * cosThetaI(subtension)
        // account for cosine subtension = just albedo :)
        // not yet
        return m_reflectance;
    }

    float LambertianBSDF::pdf(const BSDFSample& bsdfSample) const
    {
        return InvPI /* * subtension cosine*/;
    }
}