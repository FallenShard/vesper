#pragma once

#include <algorithm>
#include <eigen/Dense>

namespace vesp
{
    struct RgbSpectrum : public Eigen::Array3f
    {
        using Base = Eigen::Array3f;

        RgbSpectrum(float value = 0.f)
            : Base(value, value, value)
        {
        }

        RgbSpectrum(float red, float green, float blue)
            : Base(red, green, blue)
        {
        }

        template <typename Derived>
        RgbSpectrum(const Eigen::ArrayBase<Derived>& p)
            : Base(p)
        {
        }

        template <typename Derived>
        RgbSpectrum& operator=(const Eigen::ArrayBase<Derived>& p)
        {
            this->Base::operator=(p);
            return *this;
        }

        inline float& r() { return x(); }
        inline float& g() { return y(); }
        inline float& b() { return z(); }

        inline const float& r() const { return x(); }
        inline const float& g() const { return y(); }
        inline const float& b() const { return z(); }

        float getLuminance() const
        {
            return coeff(0) * 0.212671f + coeff(1) * 0.715160f + coeff(2) * 0.072169f;
        }

        RgbSpectrum clamp() const
        {
            return RgbSpectrum(std::max(0.f, x()), std::max(0.f, y()), std::max(0.f, z()));
        }

        bool isValid() const
        {
            for (int i = 0; i < 3; ++i)
            {
                float val = coeff(i);
                if (val < 0 || !std::isfinite(val))
                    return false;
            }
            return true;
        }

        RgbSpectrum toSrgb() const
        {
            RgbSpectrum result;

            for (int i = 0; i < 3; ++i)
            {
                float val = coeff(i);
                if (val <= 0.0031308f)
                    result[i] = 12.92f * val;
                else
                    result[i] = (1.f + 0.055f) * std::pow(val, 1.f / 2.4f) - 0.055f;
            }

            return result;
        }

        RgbSpectrum toLinearRgb() const
        {
            RgbSpectrum result;

            for (int i = 0; i < 3; ++i)
            {
                float val = coeff(i);
                if (val <= 0.04045f)
                    result[i] = val * (1.f / 12.92f);
                else
                    result[i] = std::pow((val + 0.055f) * (1.f / 1.055f), 2.4f);
            }

            return result;
        }
    };
}