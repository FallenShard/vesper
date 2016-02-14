#pragma once

#include <vector>

#include <math/Types.hpp>

#include <spectrum/Spectrum.hpp>

class ImageBlock
{
public:
    ImageBlock(const Vector2i& size);

    ~ImageBlock();

    void initialize(const Vector2i& size);
    void clear() {};

    Vector2i getSize() const;

    Spectrum& operator()(int row, int col)
    {
        return m_pixels.coeffRef(row, col);
    }

    std::vector<float> getRaw();
    

private:
    Eigen::Array<Spectrum, Eigen::Dynamic, Eigen::Dynamic> m_pixels;
};