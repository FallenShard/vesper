#include <algorithm>
#include "core\Bitmap.hpp"

namespace vesp
{
    Bitmap::Bitmap(int width, int height)
        : m_pixels(height * width)
        , m_width(width)
        , m_height(height)
    {
        updateBlock(0, 0, m_width - 1, m_height - 1, 0.5f);
        updateBlock(m_width / 2, 0, m_width - 1, m_height - 1, 0.2f);
        updateBlock(0, m_height / 2, m_width - 1, m_height - 1, 0.8f);
    }

    Bitmap::~Bitmap()
    {
    }

    void Bitmap::updateBlock(int left, int bottom, int width, int height, float val)
    {
        updateBlock(left, bottom, width, height, val, val + 0.2f, val - 0.1f);
    }

    void Bitmap::updateBlock(int left, int bottom, int width, int height, float r, float g, float b)
    {
        int xLo = std::max(0, left);
        int xHi = std::min(m_width - 1, left + width - 1);
        int yLo = std::max(0, bottom);
        int yHi = std::min(m_height - 1, bottom + height - 1);

        for (int y = yLo; y <= yHi; y++)
        {
            for (int x = xLo; x <= xHi; x++)
            {
                m_pixels[y * m_width + x].r = r;
                m_pixels[y * m_width + x].g = g;
                m_pixels[y * m_width + x].b = b;
            }
        }
    }

    const float* Bitmap::getRawPixels() const
    {
        return reinterpret_cast<const float*>(m_pixels.data());
    }

    std::vector<float> Bitmap::getRawPixelBlock(int left, int bottom, int width, int height) const
    {
        std::vector<float> pixelData;
        pixelData.reserve(width * height * 3);

        int xLo = std::max(0, left);
        int xHi = std::min(m_width - 1, left + width - 1);
        int yLo = std::max(0, bottom);
        int yHi = std::min(m_height - 1, bottom + height - 1);

        for (int y = yLo; y <= yHi; y++)
        {
            for (int x = xLo; x <= xHi; x++)
            {
                pixelData.push_back(m_pixels[y * m_width + x].r);
                pixelData.push_back(m_pixels[y * m_width + x].g);
                pixelData.push_back(m_pixels[y * m_width + x].b);
            }
        }
        return pixelData;
    }

    int Bitmap::getWidth() const
    {
        return m_width;
    }

    int Bitmap::getHeight() const
    {
        return m_height;
    }
}