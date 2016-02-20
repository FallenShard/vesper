#include <math/Operations.hpp>

namespace vesp
{
    const float PI         = 3.14159265358979323846f;
    const float InvPI      = 0.31830988618379067154f;
    const float InvTwoPI   = 0.15915494309189533577f;
    const float InvFourPI  = 0.07957747154594766788f;
    const float SqrtTwo    = 1.41421356237309504880f;
    const float InvSqrtTwo = 0.70710678118654752440f;

    float degToRad(float value)
    {
        return value * (PI / 180.f);
    }

    void coordinateSystem(const Vector3f& v1, Vector3f& v2, Vector3f& v3)
    {
        if (std::fabsf(v1.x()) > std::fabsf(v1.y()))
        {
            float invLen = 1.f / sqrtf(v1.x() * v1.x() + v1.z() + v1.z());
            v2 = Vector3f(v1.z() * invLen, 0.f, -v1.x() * invLen);
        }
        else
        {
            float invLen = 1.f / sqrtf(v1.y() * v1.y() + v1.z() + v1.z());
            v2 = Vector3f(0.f, v1.z() * invLen, -v1.y() * invLen);
        }
        v3 = v2.cross(v1);
    }

    Normal3f faceForward(const Normal3f& n, const Vector3f& v)
    {
        return v.dot(n) < 0.f ? -n : n;
    }
}