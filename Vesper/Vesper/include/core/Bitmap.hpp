#pragma once

#include <vector>

#include <eigen/Dense>

namespace vesp
{
    class Bitmap
    {
    public:
        Bitmap(int width, int height);
        ~Bitmap();

        void updateBlock(int left, int bottom, int width, int height, float val);
        void updateBlock(int left, int bottom, int width, int height, float r, float g, float b);

        const float* getRawPixels() const;
        std::vector<float> getRawPixelBlock(int left, int bottom, int width, int height) const;

        int getWidth() const;
        int getHeight() const;

    private:

        //Eigen::

        struct Color
        {
            float r, g, b;
        };

        std::vector<Color> m_pixels;
        int m_width;
        int m_height;
    };
}