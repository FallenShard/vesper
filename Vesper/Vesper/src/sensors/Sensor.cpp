#include <sensors/Sensor.hpp>

namespace vesp
{
    const Vector2i& Sensor::getImageSize() const
    {
        return m_imageSize;
    }
}