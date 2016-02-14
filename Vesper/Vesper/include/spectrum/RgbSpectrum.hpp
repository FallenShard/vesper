#pragma once

namespace vesp
{
    struct RgbSpectrum
    {
        float r, g, b;

        RgbSpectrum()
        {

        }

        RgbSpectrum(float rr, float gg, float bb)
            : r(rr), g(gg), b(bb)
        {

        }
    };
}