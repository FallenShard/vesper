#pragma once

#include <reconstruction/ReconstructionFilter.hpp>

namespace vesp
{
    class GaussianFilter : public ReconstructionFilter
    {
    public:
        GaussianFilter(const AttributeList& attributes);
        virtual float eval(float x, float y) const override;

    private:
        float m_stdDev;
    };
}