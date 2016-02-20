#include "core/ImageBlock.hpp"

namespace vesp
{
    ImageBlock::ImageBlock(const Vector2i& size)
        : m_pixels(size.y(), size.x())
    {
    }

    ImageBlock::~ImageBlock()
    {
    }

    void ImageBlock::initialize(const Vector2i& size)
    {
        m_pixels.resize(size.y(), size.x());
    }

    void ImageBlock::clear()
    {
        for (int i = 0; i < m_pixels.rows(); i++)
        {
            for (int j = 0; j < m_pixels.cols(); j++)
            {
                m_pixels.coeffRef(i, j) = Spectrum(0.f);
            }
        }
    }

    Vector2i ImageBlock::getSize() const
    {
        return Vector2i(static_cast<int>(m_pixels.cols()), static_cast<int>(m_pixels.rows()));
    }

    std::vector<float> ImageBlock::getRaw()
    {
        std::vector<float> data;
        data.reserve(m_pixels.size() * 3);

        for (int i = 0; i < m_pixels.rows(); i++)
        {
            for (int j = 0; j < m_pixels.cols(); j++)
            {
                auto val = m_pixels.coeff(i, j);
                data.push_back(val.r());
                data.push_back(val.g());
                data.push_back(val.b());
            }
        }

        return data;
    }
}