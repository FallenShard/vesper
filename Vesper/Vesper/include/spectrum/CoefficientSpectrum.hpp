#pragma once

#include <eigen/Dense>

template <int numSamples>
class CoefficientSpectrum : public Eigen::Array<float, numSamples, 1>
{
public:
    CoefficientSpectrum(float value)
        : Base(value)
    {
    }

private:
    using Base = Eigen::Array<float, numSamples, 1>;
};