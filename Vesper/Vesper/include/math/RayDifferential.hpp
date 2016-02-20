#pragma once

#include <math/Ray.hpp>

namespace vesp
{
    template <typename PointType, typename DirType>
    struct RayDifferential : public Ray<PointType, DirType>
    {
        using Ray = Ray<PointType, DirType>;
        using Scalar = typename PointType::Scalar;

        bool hasDifferentials;
        PointType rxOrigin, ryOrigin;
        DirType rxDir, ryDir;

        RayDifferential()
            : hasDifferentials(false)
        {
        }

        RayDifferential(const PointType& o, const DirType& d, Scalar minT, Scalar maxT, float time)
            : Ray(o, d, minT, maxT, time)
            , hasDifferentials(false)
        {
        }

        explicit RayDifferential(const Ray& ray)
            : Ray(ray)
            , hasDifferentials(false)
        {
        }

        void scaleDifferentials(float s)
        {
            rxOrigin = o + (rxOrigin - o) * s;
            ryOrigin = o + (ryOrigin - o) * s;
            rxDir = d + (rxDir - d) * s;
            ryDir = d + (ryDir - d) * s;
        }
    };
}