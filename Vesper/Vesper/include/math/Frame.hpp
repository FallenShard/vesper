#pragma once

#include <math/Types.hpp>
#include <math/Operations.hpp>

namespace vesp
{
    struct Frame
    {
        Normal3f n;
        Vector3f s, t;

        Frame()
        {
        }

        Frame(const Normal3f& n, const Vector3f& s, const Vector3f& t)
            : n(n)
            , s(s)
            , t(t)
        {
        }

        Frame(const Vector3f& x, const Vector3f& y, const Vector3f& z)
            : n(z)
            , s(x)
            , t(y)
        {
        }

        inline Vector3f toLocal(const Vector3f& v) const
        {
            return Vector3f(v.dot(s), v.dot(t), v.dot(n));
        }

        inline Vector3f toWorld(const Vector3f& v) const
        {
            return s * v.x() + t * v.y() + n * v.z();
        }

        inline static float cosTheta(const Vector3f& v)
        {
            return v.z();
        }

        inline static float sinTheta(const Vector3f& v)
        {
            float sinTheta2 = 1.f - v.z() * v.z();
            if (sinTheta2 <= 0.f) return 0.f;

            return std::sqrtf(sinTheta2);
        }

        inline static float tanTheta(const Vector3f& v)
        {
            float sinTheta2 = 1.f - v.z() * v.z();
            if (sinTheta2 <= 0.f) return 0.f;

            return std::sqrtf(sinTheta2) / v.z();
        }

        inline static float sinTheta2(const Vector3f& v)
        {
            return 1.f - v.z() * v.z();
        }

        inline static float sinPhi(const Vector3f& v)
        {
            float sinTheta = Frame::sinTheta(v);
            if (sinTheta == 0.f) return 1.f;

            return clamp(v.y() / sinTheta, -1.f, 1.f);
        }

        inline static float cosPhi(const Vector3f& v)
        {
            float sinTheta = Frame::sinTheta(v);
            if (sinTheta == 0.f) return 1.f;

            return clamp(v.x() / sinTheta, -1.f, 1.f);
        }

        inline static float sinPhi2(const Vector3f& v)
        {
            return clamp(v.y() * v.y() / Frame::sinTheta2(v), 0.f, 1.f);
        }

        inline static float cosPhi2(const Vector3f& v)
        {
            return clamp(v.x() * v.x() / Frame::sinTheta2(v), 0.f, 1.f);
        }

        bool operator==(const Frame& frame) const
        {
            return frame.s == s && frame.t == t && frame.n == n;
        }

        bool operator!=(const Frame& frame) const
        {
            return !operator==(frame);
        }
    };
}