#pragma once

#include <bsdfs/BSDF.hpp>

namespace vesp
{
    class MirrorBSDF : public BSDF
    {
    public:
        MirrorBSDF(const AttributeList& attributes);
        ~MirrorBSDF();

        virtual Spectrum eval(const BSDFSample& bsdfSample) const override;
        virtual Spectrum sample(BSDFSample& bsdfSample, Sampler& sampler) const override;
        virtual float pdf(const BSDFSample& bsdfSample) const override;
        virtual unsigned int getType() const override;
    };
}