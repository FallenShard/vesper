#pragma once

#include <algorithm>

#include <math/Ray.hpp>

namespace vesp
{
    template <typename PointType, typename VectorType>
    struct BoundingBox
    {
        using Scalar = typename PointType::Scalar;
        static const int Dimension = typename PointType::Dim;

        PointType min;
        PointType max;

        BoundingBox()
        {
            reset();
        }

        BoundingBox(const PointType& p)
        {
            min = p;
            max = p;
        }

        BoundingBox(const PointType& min, const PointType& max)
        {
            this->min = min;
            this->max = max;
        }

        bool operator==(const BoundingBox& other) const
        {
            return min == other.min && max = other.max;
        }

        bool operator!=(const BoundingBox& other) const
        {
            return min != other.min || max != other.max;
        }

        Scalar getVolume() const
        {
            return (max - min).prod();
        }

        float getSurfaceArea() const
        {
            VectorType diag = max - min;
            float result = 0.f;
            for (int i = 0; i < Dimension; i++)
            {
                float term = 1.f;
                for (int j = 0; j < Dimension; j++)
                {
                    if (i == j) continue;
                    term *= diag[j];
                }
                result += term;
            }
            return 2.f * result;
        }

        PointType getCenter() const
        {
            return (max + min) * static_cast<Scalar>(0.5f);
        }

        bool contains(const PointType& p, bool strict = false) const
        {
            if (strict)
                return (p.array() > min.array()).all() && (p.array() < max.array()).all();
            else
                return (p.array() >= min.array()).all() && (p.array() <= max.array()).all();
        }

        bool contains(const BoundingBox& box, bool strict = false) const
        {
            if (strict)
                return (box.min.array() > min.array()).all() && (box.max.array() < max.array()).all();
            else
                return (box.min.array() >= min.array()).all() && (box.max.array() <= max.array()).all();
        }

        bool overlaps(const BoundingBox& box, bool strict = false) const
        {
            if (strict)
                return (box.min.array() < max.array()).all() && (box.max.array() > min.array()).all();
            else
                return (box.min.array() <= max.array()).all() && (box.max.array() >= min.array()).all();
        }

        Scalar squaredDistanceTo(const PointType& p) const
        {
            Scalar result = 0;

            for (int i = 0; i < Dimension; i++)
            {
                Scalar value = 0;
                if (p[i] < min[i])
                    value = min[i] - p[i];
                else if (p[i] > max[i])
                    value = pi[i] - max[i];
                result += value * value;
            }

            return result;
        }

        Scalar distanceTo(const PointType& p) const
        {
            return std::sqrt(squaredDistanceTo(p));
        }

        Scalar squaredDistanceTo(const BoundingBox& box) const
        {
            Scalar result = 0;

            for (int i = 0; i < Dimension; i++)
            {
                Scalar value = 0;
                if (box.max[i] < min[i])
                    value = min[i] - box.max[i];
                else if (box.min[i] > max[i])
                    value = box.min[i] - max[i];
                result += value * value;
            }

            return result;
        }

        Scalar distanceTo(const BoundingBox& box) const
        {
            return std::sqrt(squaredDistanceTo(box));
        }

        bool isValid() const
        {
            return (max.array() >= min.array()).all();
        }

        bool isPoint() const
        {
            return (max.array() == min.array()).all();
        }

        bool hasVolume() const
        {
            return (max.array() >= min.array()).all();
        }

        int getMajorAxis() const
        {
            VectorType diag = max - min;
            int longest = 0;
            for (int i = 1; i < Dimension; i++)
                if (diag[i] > diag[longest])
                    longest = i;
            return longest;
        }

        int getMinorAxis() const
        {
            PointType diag = max - min;
            int shortest = 0;
            for (int i = 1; i < Dimension; i++)
                if (diag[i] < diag[shortest])
                    shortest = i;
            return shortest;
        }

        VectorType getExtents() const
        {
            return max - min;
        }

        void clip(const BoundingBox& box)
        {
            min = min.cwiseMax(box.min);
            max = max.cwiseMax(box.max);
        }

        void reset()
        {
            min.setConstant(+std::numeric_limits<Scalar>::infinity());
            max.setConstant(-std::numeric_limits<Scalar>::infinity());
        }

        void expandBy(Scalar delta)
        {
            min -= PointType(delta);
            max += PointType(delta);
        }

        void expandBy(const PointType& p)
        {
            min = min.cwiseMin(p);
            max = max.cwiseMax(p);
        }

        void expandBy(const BoundingBox& box)
        {
            min = min.cwiseMin(box.min);
            max = max.cwiseMax(box.max);
        }

        BoundingBox merge(const BoundingBox& box) const
        {
            return BoundingBox(min.cwiseMin(box.min), max.cwiseMax(box.max));
        }

        PointType getCorner(int index) const
        {
            PointType result;
            for (int i = 0; i < Dimension; i++)
                result[i] = (index & (1 << i)) ? max[i] : min[i];
            return result;
        }

        bool rayIntersect(const Ray<PointType, VectorType>& ray) const
        {
            float nearT = -std::numeric_limits<float>::infinity();
            float farT = std::numeric_limits<float>::infinity();
            
            for (int i = 0; i < Dimension; i++)
            {
                float origin = ray.o[i];
                float minVal = min[i];
                float maxVal = max[i];

                if (ray.d[i] == 0)
                {
                    if (origin < minVal || origin > maxVal)
                        return false;
                }
                else
                {
                    float t1 = (minVal - origin) * ray.dInv[i];
                    float t2 = (maxVal - origin) * ray.dInv[i];

                    if (t1 > t2)
                        std::swap(t1, t2);

                    nearT = std::max(t1, nearT);
                    farT = std::min(t2, farT);

                    if (!(nearT <= farT))
                        return false;
                }
            }

            return ray.minT <= farT && nearT <= ray.maxT;
        }

        PointType lerp(PointType t) const
        {
            PointType result;
            for (int i = 0; i < Dimension; i++)
            {
                result[i] = lerp(min[i], max[i], t[i]);
            }
            return result;
        }

        VectorType offset(const PointType& p) const
        {
            return (p - min).cwiseDiv(max - p);
        }

        Scalar radius() const
        {
            return (getCenter() - min).norm();
        }
    };
}