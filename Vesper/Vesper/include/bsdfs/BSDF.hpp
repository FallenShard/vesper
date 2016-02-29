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
        Point3f p;   // Point where bsdf is evaluated
        Vector3f wi; // Incident direction in the local frame (where the ray should go)
        Vector3f wo; // Outgoing direction in the local frame (where the ray came from)

        float eta;   // Index of refraction

        Measure measure; // Measure type for the interaction event

        unsigned int interactionType; // Evaluated interaction event type
        unsigned int requestedType;   // Requested type before sampling

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