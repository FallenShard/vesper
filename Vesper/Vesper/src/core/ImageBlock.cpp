#include <core/ImageBlock.hpp>
#include <reconstruction/ReconstructionFilter.hpp>

namespace vesp
{
    ImageBlock::ImageBlock()
    {
    }

    ImageBlock::ImageBlock(const Vector2i& size, const ReconstructionFilter* filter)
        : m_offset(0, 0)
    {
        initialize(size, filter);
    }

    ImageBlock::ImageBlock(const Point2i& offset, const Vector2i& size, const ReconstructionFilter* filter)
    {
        initialize(offset, size, filter);
    }

    ImageBlock::~ImageBlock()
    {
    }

    void ImageBlock::initialize(const Vector2i& size, const ReconstructionFilter* filter)
    {
        m_size = size;
        if (filter)
        {
            m_filter = filter;
            m_borderSize = static_cast<int>(std::ceil(m_filter->getRadius() - 0.5f));
            int wSize = static_cast<int>(std::ceil(2 * m_filter->getRadius())) + 1;
            m_weights.resize(wSize);
            for (int i = 0; i < wSize; i++)
            {
                m_weights[i].resize(wSize);
                for (int j = 0; j < wSize; j++)
                    m_weights[i][j] = 0.f;
            }
        }
        else
        {
            m_borderSize = 0;
            m_filter = nullptr;
        }

        m_pixels.resize(size.y() + 2 * m_borderSize, size.x() + 2 * m_borderSize);
    }

    void ImageBlock::initialize(const Point2i& offset, const Vector2i& size, const ReconstructionFilter* filter)
    {
        m_offset = offset;
        initialize(size, filter);
    }

    void ImageBlock::clear()
    {
        for (int i = 0; i < m_pixels.rows(); i++)
        {
            for (int j = 0; j < m_pixels.cols(); j++)
            {
                m_pixels.coeffRef(i, j) = WeightedSpectrum(0.f);
            }
        }
    }

    Point2i ImageBlock::getOffset() const
    {
        return m_offset;
    }

    Vector2i ImageBlock::getSize() const
    {
        return m_size;
    }

    Vector2i ImageBlock::getFullSize() const
    {
        return m_size + Vector2i(m_borderSize, m_borderSize);
    }

    void ImageBlock::addSample(const Point2f& pixelSample, const Spectrum& radiance)
    {
        if (!radiance.isValid())
        {
            std::cerr << "Integrator computed an invalid radiance value!\n";
            return;
        }

        Point2f pos(
            pixelSample.x() - 0.5f - (m_offset.x() - m_borderSize),
            pixelSample.y() - 0.5f - (m_offset.y() - m_borderSize));

        int xLo = std::max(0, static_cast<int>(std::ceil(pos.x() - m_filter->getRadius())));
        int yLo = std::max(0, static_cast<int>(std::ceil(pos.y() - m_filter->getRadius())));
        int xHi = std::min(static_cast<int>(m_pixels.cols() - 1), static_cast<int>(std::floor(pos.x() + m_filter->getRadius())));
        int yHi = std::min(static_cast<int>(m_pixels.rows() - 1), static_cast<int>(std::floor(pos.y() + m_filter->getRadius())));

        for (int y = yLo, wy = 0; y <= yHi; ++y, ++wy)
            for (int x = xLo, wx = 0; x <= xHi; ++x, ++wx)
                m_weights[wy][wx] = m_filter->evalDiscrete(x - pos.x(), y - pos.y());

        for (int y = yLo, wy = 0; y <= yHi; ++y, ++wy)
            for (int x = xLo, wx = 0; x <= xHi; ++x, ++wx)
                m_pixels.coeffRef(y, x) += WeightedSpectrum(radiance) * m_weights[wy][wx];
    }

    std::vector<float> ImageBlock::getRaw()
    {
        std::vector<float> data;
        data.reserve(m_size.prod() * 3);

        for (int i = m_borderSize; i <  m_borderSize + m_size.y(); i++)
        {
            for (int j = m_borderSize; j < m_borderSize + m_size.x(); j++)
            {
                auto val = m_pixels.coeff(i, j);
                auto rgb = val.toRgb();
                data.push_back(rgb.r());
                data.push_back(rgb.g());
                data.push_back(rgb.b());
            }
        }

        return data;
    }

    ImageBlock::Descriptor::Descriptor(int xOffset, int yOffset, int width, int height)
    {
        offset = Point2i(xOffset, yOffset);
        size = Vector2i(width, height);
    }

    ImageBlock::Descriptor::Descriptor()
    {
        offset = Point2i(0, 0);
        size = Vector2i(0, 0);
    }
}