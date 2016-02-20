#pragma once

#include <bsdfs/BSDF.hpp>

namespace vesp
{
    class LambertianBSDF : public BSDF
    {
    public:
        LambertianBSDF(const AttributeList& attributes);
        ~LambertianBSDF();

        virtual Spectrum eval(const BSDFSample& bsdfSample) const override;
        virtual Spectrum sample(BSDFSample& bsdfSample, const Sampler& sampler) const override;
        virtual float pdf(const BSDFSample& bsdfSample) const override;
        virtual unsigned int getType() const override;

    private:
        Spectrum m_reflectance;

    };
}
