#pragma once

#include <core/VesperObject.hpp>

namespace vesp
{
    class Sampler;
    class Shape;

    struct EmitterSample
    {
        Point3f ref; // In(eval, sample) - Vantage point that is being illuminated
        Point3f p;   // In(eval), Out(sample) - Point on the emitter
        Normal3f n;  // In(eval), Out(sample) - Normal at the emitter point
        Vector3f wi; // In(eval), Out(sample) - Vantage-to-emitter direction

        float pdf;   // In(eval), Out(sample) - Probability of selecting sampled emitter point

        Ray3f shadowRay; // Out(sample) - Accompanying shadow ray

        EmitterSample() {}

        // To be used when sampling the emitter
        EmitterSample(const Point3f& ref) : ref(ref) {}

        // To be used when evaluating the emitter
        EmitterSample(const Point3f& ref, const Point3f& p, const Normal3f& n) : ref(ref), p(p), n(n) {}
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
        
        virtual Spectrum samplePhoton(Ray3f& ray, Sampler& sampler) const = 0;

        virtual ClassType getClassType() const override { return ClassType::Emitter; }

    protected:
        Shape* m_shape;
    };
}