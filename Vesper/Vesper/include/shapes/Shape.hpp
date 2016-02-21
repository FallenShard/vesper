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

    class Shape : public VesperObject
    {
    public:
        virtual unsigned int submitGeometry(GeometryVisitor* visitor) = 0;

        virtual void setIntersectionInfo(unsigned int triangleId, const Ray3f& ray, Intersection& its) const = 0;

        virtual void addChild(std::shared_ptr<VesperObject> child) override;

        const BSDF* getBSDF() const;

        const Emitter* getEmitter() const;

        std::shared_ptr<Emitter> getEmitterManaged() const;

        virtual ClassType getClassType() const override { return ClassType::Shape; }

    private:
        std::shared_ptr<BSDF> m_bsdf;
        std::shared_ptr<Emitter> m_emitter;
    };
}