#pragma once

namespace vesp
{
    const static float PI         = 3.14159265358979323846f;
    const static float InvPI      = 0.31830988618379067154f;
    const static float InvTwoPI   = 0.15915494309189533577f;
    const static float InvFourPI  = 0.07957747154594766788f;
    const static float SqrtTwo    = 1.41421356237309504880f;
    const static float InvSqrtTwo = 0.70710678118654752440f;

    template <typename T>
    inline T clamp(T value, T minVal, T maxVal)
    {
        if (value > maxVal) return maxVal;
        if (value < minVal) return minVal;

        return value;
    }

    inline float degToRad(float value)
    {
        return value * (PI / 180.f);
    }
}