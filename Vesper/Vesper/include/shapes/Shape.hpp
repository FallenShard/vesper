#pragma once

#include <math/Types.hpp>
#include <math/Frame.hpp>

#include <shapes/Intersection.hpp>
#include <core/VesperObject.hpp>

namespace vesp
{
    class GeometryVisitor;
    class BSDF;
    class Emitter;
    class Sampler;

    struct ShapeSample
    {
        Point3f ref; // Vantage point for the sample
        Point3f p;   // Sampled point on the shape surface
        Normal3f n;  // Normal at the sampled point
        float pdf;   // pdf of the sample

        ShapeSample() {}
        ShapeSample(const Point3f& ref) : ref(ref) {}
        ShapeSample(const Point3f& ref, const Point3f& p) : ref(ref), p(p) {}
    };

    class Shape : public VesperObject
    {
    public:
        virtual void addChild(std::shared_ptr<VesperObject> child) override;
        virtual void configure() override;

        virtual unsigned int submitGeometry(GeometryVisitor* visitor) = 0;

        virtual void setIntersectionInfo(unsigned int triangleId, const Ray3f& ray, Intersection& its) const = 0;
        virtual void sampleSurface(ShapeSample& shapeSample, Sampler& sampler) const = 0;
        virtual float pdfSurface(const ShapeSample& shapeSample) const = 0;

        const BSDF* getBSDF() const;
        const Emitter* getEmitter() const;
        std::shared_ptr<Emitter> getEmitterManaged() const;

        virtual ClassType getClassType() const override { return ClassType::Shape; }

    protected:
        std::shared_ptr<BSDF> m_bsdf;
        std::shared_ptr<Emitter> m_emitter;

        Transform m_toWorld;
    };
}