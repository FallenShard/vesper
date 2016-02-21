#pragma once

#include <core/VesperObject.hpp>

namespace vesp
{
    class Sampler;
    class Shape;

    struct EmitterSample
    {
        Point3f ref; // Vantage point
        Point3f p;   // Point on the emitter
        Normal3f n;  // Normal at the emitter point
        Vector3f wi; // vantage-to-emitter direction

        float pdf;   // Probability of selecting sampled emitter point

        Ray3f shadowRay; // Accompanying shadow ray
    };

    class Emitter : public VesperObject
    {
    public:
        virtual ~Emitter() {};

        virtual void setSceneBoundingBox(const BoundingBox3f& bounds) {};
        virtual bool isOnSurface() { return true; }
        void setShape(Shape* shape) { m_shape = shape; }

        virtual Spectrum eval(const EmitterSample& emitterSample) const = 0;
        virtual Spectrum sample(EmitterSample& emitterSample, Sampler& sampler) const = 0;
        virtual float pdf(const EmitterSample& emitterSample) const = 0;
        
        virtual Spectrum samplePhoton(Ray3f& ray, const Sampler& sampler) const = 0;

        virtual ClassType getClassType() const override { return ClassType::Emitter; }

    protected:
        Shape* m_shape;
    };
}