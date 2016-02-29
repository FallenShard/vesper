#pragma once

#include <core/VesperObject.hpp>

namespace vesp
{
    class Sampler;

    enum class Measure
    {
        Unknown,
        SolidAngle,
        Discrete
    };

    enum BSDFType
    {
        Passthrough,
        Diffuse,
        Glossy,
        Delta
    };

    struct BSDFSample
    {
        Point3f p;   // In(eval, sample) - Point where bsdf is evaluated
        Vector3f wi; // In(eval, sample) - Incident direction in the local frame (where the ray came from)
        Vector3f wo; // In(eval), Out(sample) - Outgoing direction in the local frame (where the ray is going)

        float eta;   // In(eval), Out(sample) - Index of refraction

        Measure measure;              // In(eval), Out(sample) - Measure type for the interaction event
        unsigned int requestedType;   // In(eval), Out(sample) - Requested type before sampling
        unsigned int interactionType; // In(eval), Out(sample) - Evaluated interaction event type
        

        BSDFSample() {}
        BSDFSample(const Point3f& p, const Vector3f& localWi) : p(p), wi(localWi) {}
        BSDFSample(const Point3f& p, const Vector3f& localWi, const Vector3f& localWo) : p(p), wi(localWi), wo(localWo) {}
    };

    class BSDF : public VesperObject
    {
    public:
        virtual ~BSDF() {};
        virtual Spectrum eval(const BSDFSample& bsdfSample) const = 0;
        virtual Spectrum sample(BSDFSample& bsdfSample, Sampler& sampler) const = 0;
        virtual float pdf(const BSDFSample& bsdfSample) const = 0;
        virtual unsigned int getType() const = 0;

        virtual ClassType getClassType() const override { return ClassType::BSDF; }
    };
}