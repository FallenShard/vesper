#pragma once

#include <math/Types.hpp>
#include <math/Frame.hpp>

#include <shapes/Intersection.hpp>
#include <core/VesperObject.hpp>

#include <bsdfs/BSDF.hpp>

namespace vesp
{
    class GeometryVisitor;

    class Shape : public VesperObject
    {
    public:
        virtual unsigned int submitGeometry(GeometryVisitor* visitor) = 0;

        virtual void setIntersectionInfo(unsigned int triangleId, const Ray3f& ray, Intersection& its) const = 0;

        virtual void addChild(std::shared_ptr<VesperObject> child) override;

        const BSDF* getBSDF() const;

    private:
        std::shared_ptr<BSDF> m_bsdf;
    };
}