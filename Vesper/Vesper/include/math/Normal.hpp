#pragma once

#include <eigen/Core>

namespace vesp
{
    template <typename T, int dimension>
    struct Normal : public Eigen::Matrix<T, dimension, 1>
    {
        Normal(T value = static_cast<T>(0))
        {
            Base::setConstant(value);
        }

        Normal(T x, T y)
            : Base(x, y)
        {
        }

        Normal(T x, T y, T z)
            : Base(x, y, z)
        {
        }

        Normal(T x, T y, T z, T w)
            : Base(x, y, z, w)
        {
        }

        template <typename Derived>
        Normal(const Eigen::MatrixBase<Derived>& p)
            : Base(p)
        {
        }

        template <typename Derived>
        Normal& operator=(const Eigen::MatrixBase<Derived>& p)
        {
            this->base::operator=(p);
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

    private:
        using Base = Eigen::Matrix<Scalar, dimension, 1>;
    };
}