#pragma once

#include <algorithm>
#include <eigen/Dense>

#include <spectrum/RgbSpectrum.hpp>

namespace vesp
{
    struct WeightedSpectrum : public Eigen::Array4f
    {
        using Base = Eigen::Array4f;

        WeightedSpectrum(float value = 0.f)
            : Base(value, value, value, value)
        {
        }

        WeightedSpectrum(const RgbSpectrum& rgb)
            : Base(rgb.r(), rgb.g(), rgb.b(), 1.f)
        {
        }

        WeightedSpectrum(float red, float green, float blue, float weight)
            : Base(red, green, blue, weight)
        {
        }

        template <typename Derived>
        WeightedSpectrum(const Eigen::ArrayBase<Derived>& p)
            : Base(p)
        {
        }

        template <typename Derived>
        WeightedSpectrum& operator=(const Eigen::ArrayBase<Derived>& p)
        {
            this->Base::operator=(p);
            return *this;
        }

        RgbSpectrum toRgb() const
        {
            if (w() != 0)
                return head<3>() / w();
            else
                return RgbSpectrum(0.f);
        }
    };
}