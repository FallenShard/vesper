#include <sampling/Independent.hpp>

namespace vesp
{
    IndependentSampler::IndependentSampler()
        : m_randomEngine(std::random_device()())
        , m_distribution(0, 1)
    {
        m_sampleCount = 1;
    }

    IndependentSampler::~IndependentSampler()
    {
    }

    void IndependentSampler::generate()
    {
    }

    void IndependentSampler::advance()
    {
    }

    float IndependentSampler::next1D()
    {
        return nextFloat();
    }

    Point2f IndependentSampler::next2D()
    {
        return Point2f(nextFloat(), nextFloat());
    }

    float IndependentSampler::nextFloat()
    {
        return m_distribution(m_randomEngine);
    }
}