#pragma once

#include <eigen/Core>

namespace vesp
{
    template <typename T, int dimension>
    struct Point : public Eigen::Matrix<T, dimension, 1>
    {
        enum
        {
            Dim = dimension
        };

        using Scalar = T;
        using Base = Eigen::Matrix<T, dimension, 1>;

        Point(T value = static_cast<T>(0))
        {
            Base::setConstant(value);
        }

        Point(T x, T y)
            : Base(x, y)
        {
        }

        Point(T x, T y, T z)
            : Base(x, y, z)
        {
        }

        Point(T x, T y, T z, T w)
            : Base(x, y, z, w)
        {
        }

        template <typename Derived>
        Point(const Eigen::MatrixBase<Derived>& p)
            : Base(p)
        {
        }

        template <typename Derived>
        Point& operator=(const Eigen::MatrixBase<Derived>& p)
        {
            this->Base::operator=(p);
            return *this;
        }

        std::string toString() const
        {
            std::string result;
            for (size_t i = 0; i < dimension; ++i)
            {
                result += std::to_string(coeff(i));
                if (i + 1 < dimension) result += "\t";
            }
            return "[" + result + "]";
        }
    };
}