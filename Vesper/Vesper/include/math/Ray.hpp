#pragma once

#include <limits>

namespace vesp
{
    const static float RayEpsilon = 1e-4f;

    template <typename PointType, typename DirType>
    struct Ray
    {
        using Scalar = typename PointType::Scalar;

        PointType o;
        DirType d;
        DirType dInv;
        Scalar minT;
        Scalar maxT;

        Ray()
            : minT(RayEpsilon)
            , maxT(std::numeric_limits<Scalar>::infinity())
        {
        }

        Ray(const PointType& origin, const DirType& dir)
            : o(origin)
            , d(dir)
            , minT(RayEpsilon)
            , maxT(std::numeric_limits<Scalar>::infinity())
        {
            update();
        }

        Ray(const PointType& origin, const DirType& dir, Scalar minT, Scalar maxT)
            : o(origin)
            , d(dir)
            , minT(minT)
            , maxT(maxT)
        {
            update();
        }

        Ray(const Ray& ray)
            : o(ray.o)
            , d(ray.d)
            , dInv(ray.dInv)
            , minT(ray.minT)
            , maxT(ray.maxT)
        {
        }

        Ray(const Ray& ray, Scalar minT, Scalar maxT)
            : o(ray.o)
            , d(ray.d)
            , dInv(ray.dInv)
            , minT(minT)
            , maxT(maxT)
        {
        }

        void update()
        {
            dInv = d.cwiseInverse();
        }

        PointType operator() (Scalar t) const
        {
            return o + t * d;
        }

        Ray reverse() const
        {
            Ray result;
            result.o = o;
            result.d = -d;
            result.dInv = -d;
            result.minT = minT;
            result.maxT = maxT;
            return result;
        }
    };
}
