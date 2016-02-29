#pragma once

#include <bsdfs/BSDF.hpp>

namespace vesp
{
    class DielectricBSDF : public BSDF
    {
    public:
        DielectricBSDF(const AttributeList& attributes);
        ~DielectricBSDF();

        virtual Spectrum eval(const BSDFSample& bsdfSample) const override;
        virtual Spectrum sample(BSDFSample& bsdfSample, Sampler& sampler) const override;
        virtual float pdf(const BSDFSample& bsdfSample) const override;
        virtual unsigned int getType() const override;

    private:
        float m_intIOR;
        float m_extIOR;
    };
}