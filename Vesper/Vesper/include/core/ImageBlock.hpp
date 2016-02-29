#pragma once

#include <vector>

#include <math/Types.hpp>

#include <spectrum/Spectrum.hpp>
#include <spectrum/WeightedSpectrum.hpp>

namespace vesp
{
    class ReconstructionFilter;

    class ImageBlock
    {
    public:
        struct Descriptor
        {
            Point2i offset;
            Vector2i size;

            Descriptor(int xOffset, int yOffset, int width, int height);
            Descriptor();
        };

        ImageBlock();
        ImageBlock(const Vector2i& size, const ReconstructionFilter* filter);
        ImageBlock(const Point2i& offset, const Vector2i& size, const ReconstructionFilter* filter);

        ~ImageBlock();

        void initialize(const Vector2i& size, const ReconstructionFilter* filter);
        void initialize(const Point2i& offset, const Vector2i& size, const ReconstructionFilter* filter);
        void clear();

        Point2i getOffset() const;
        Vector2i getSize() const;
        Vector2i getFullSize() const;

        void addSample(const Point2f& pixelSample, const Spectrum& radiance);

        std::vector<float> getRaw();

    private:
        Eigen::Array<WeightedSpectrum, Eigen::Dynamic, Eigen::Dynamic> m_pixels;
        Point2i m_offset;
        Vector2i m_size;
        int m_borderSize;

        const ReconstructionFilter* m_filter;

        std::vector<std::vector<float>> m_weights;
    };
}