#pragma once

#include <math/Types.hpp>
#include <spectrum/Spectrum.hpp>

#include <core/VesperObject.hpp>

namespace vesp
{
    class Sensor : public VesperObject
    {
    public:
        virtual Spectrum sampleRay(Ray3f& ray, const Point2f& samplePosition, const Point2f& apertureSample) const = 0;

        const Vector2i& getImageSize() const;

    protected:
        Vector2i m_imageSize;

        // Filter
    };
}