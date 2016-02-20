#pragma once

#include <eigen/Core>

namespace vesp
{
    template <typename T, int dimension>
    struct Vector : public Eigen::Matrix<T, dimension, 1>
    {
        using Base = Eigen::Matrix<Scalar, dimension, 1>;

        Vector(T value = static_cast<T>(0))
        {
            Base::setConstant(value);
        }

        Vector(T x, T y)
            : Base(x, y)
        {
        }

        Vector(T x, T y, T z)
            : Base(x, y, z)
        {
        }

        Vector(T x, T y, T z, T w)
            : Base(x, y, z, w)
        {
        }

        template <typename Derived>
        Vector(const Eigen::MatrixBase<Derived>& p)
            : Base(p)
        {
        }

        template <typename Derived>
        Vector& operator=(const Eigen::MatrixBase<Derived>& p)
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