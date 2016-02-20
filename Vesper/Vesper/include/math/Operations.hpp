#pragma once

#include <math/Types.hpp>

namespace vesp
{
    extern const float PI;
    extern const float InvPI;
    extern const float InvTwoPI;
    extern const float InvFourPI;
    extern const float SqrtTwo;
    extern const float InvSqrtTwo;

    extern inline float degToRad(float value);

    extern inline void coordinateSystem(const Vector3f& v1, Vector3f& v2, Vector3f& v3);

    extern inline Normal3f faceForward(const Normal3f& n, const Vector3f& v);

    template <typename T>
    inline static T clamp(T value, T minVal, T maxVal)
    {
        if (value > maxVal) return maxVal;
        if (value < minVal) return minVal;

        return value;
    }

    template <typename T>
    inline static T lerp(T a, T b, float s)
    {
        return a + (b - a) * s;
    }
}