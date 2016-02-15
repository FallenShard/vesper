#include <cameras/Camera.hpp>

namespace vesp
{
    const Vector2i& Camera::getImageSize() const
    {
        return m_imageSize;
    }
}