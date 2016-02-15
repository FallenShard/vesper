#pragma once

#include <math/Types.hpp>
#include <spectrum/Spectrum.hpp>

namespace vesp
{
    class Camera
    {
    public:
        virtual Spectrum sampleRay(Ray3f& ray, const Point2f& samplePosition, const Point2f& apertureSample) const = 0;

        const Vector2i& getImageSize() const;


    protected:
        Vector2i m_imageSize;

        // Filter
    };
}