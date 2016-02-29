#include <reconstruction/GaussianFilter.hpp>

namespace vesp
{
    GaussianFilter::GaussianFilter(const AttributeList& attribs)
    {
        m_radius = attribs.getFloat("radius", 2.f);
        m_stdDev = attribs.getFloat("stddev", 0.5f);
    }

    float GaussianFilter::eval(float x, float y) const
    {
        float alpha = -1.f / (2.f * m_stdDev * m_stdDev);
        float evalX = std::max(0.f, std::exp(alpha * x * x) - std::exp(alpha * m_radius * m_radius));
        float evalY = std::max(0.f, std::exp(alpha * y * y) - std::exp(alpha * m_radius * m_radius));
        return evalX * evalY;
    }
}