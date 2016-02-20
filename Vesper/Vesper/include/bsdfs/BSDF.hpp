#pragma once

#include <core/VesperObject.hpp>

namespace vesp
{
    class Sampler;

    struct BSDFSample
    {
        Point3f p;
        Vector3f wi;
        Vector3f wo;
    };

    class BSDF : public VesperObject
    {
    public:
        virtual ~BSDF() {};
        virtual Spectrum eval(const BSDFSample& bsdfSample) const = 0;
        virtual Spectrum sample(BSDFSample& bsdfSample, const Sampler& sampler) const = 0;
        virtual float pdf(const BSDFSample& bsdfSample) const = 0;
    };
}