#pragma once

namespace vesp
{
    struct RgbSpectrum
    {
        float r, g, b;

        RgbSpectrum()
        {
        }

        RgbSpectrum(float value)
            : r(value)
            , g(value)
            , b(value)
        {
        }

        RgbSpectrum(float red, float green, float blue)
            : r(red)
            , g(green)
            , b(blue)
        {
        }
    };
}