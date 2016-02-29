#pragma once

#include <math/Types.hpp>
#include <spectrum/Spectrum.hpp>

#include <core/VesperObject.hpp>

namespace vesp
{
    class ReconstructionFilter;

    class Sensor : public VesperObject
    {
    public:
        virtual void addChild(std::shared_ptr<VesperObject> child) override;
        virtual Spectrum sampleRay(Ray3f& ray, const Point2f& samplePosition, const Point2f& apertureSample) const = 0;

        const Vector2i& getImageSize() const;
        const ReconstructionFilter* getReconstructionFilter() const;

        virtual ClassType getClassType() const override { return ClassType::Sensor; }

    protected:
        Vector2i m_imageSize;

        std::shared_ptr<ReconstructionFilter> m_filter;
    };
}