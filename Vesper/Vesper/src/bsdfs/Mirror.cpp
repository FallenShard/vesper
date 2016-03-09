#include <bsdfs/Mirror.hpp>
#include <math/Frame.hpp>
#include <sampling/Sampler.hpp>

namespace vesp
{
    MirrorBSDF::MirrorBSDF(const AttributeList& attributes)
    {
    }

    MirrorBSDF::~MirrorBSDF()
    {
    }

    Spectrum MirrorBSDF::eval(const BSDFSample& bsdfSample) const
    {
        return Spectrum(0.f);
    }

    Spectrum MirrorBSDF::sample(BSDFSample& bsdfSample, Sampler& sampler) const
    {
        bsdfSample.eta = 1.f;
        bsdfSample.interactionType = BSDFType::Delta;
        bsdfSample.measure = Measure::Discrete;

        // Reflection in local coordinates
        bsdfSample.wo = Vector3f(-bsdfSample.wi.x(), -bsdfSample.wi.y(), bsdfSample.wi.z());

        return Spectrum(1.f);
    }

    float MirrorBSDF::pdf(const BSDFSample& bsdfSample) const
    {
        return 0.f;
    }

    unsigned int MirrorBSDF::getType() const
    {
        return BSDFType::Delta;
    }
}