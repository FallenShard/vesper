
#include <sampling/Independent.hpp>

namespace vesp
{
    IndependentSampler::IndependentSampler(const AttributeList& attributes)
        : m_randomEngine(std::random_device()())
        , m_distribution(0, 1)
    {
        m_sampleCount = attributes.getInteger("numSamples", 64);
    }

    IndependentSampler::IndependentSampler()
        : m_randomEngine(std::random_device()())
        , m_distribution(0, 1)
    {
        m_sampleCount = 1;
    }

    IndependentSampler::~IndependentSampler()
    {
    }

    std::unique_ptr<Sampler> IndependentSampler::clone() const
    {
        std::unique_ptr<IndependentSampler> other = std::make_unique<IndependentSampler>();
        other->m_randomEngine = m_randomEngine;
        other->m_distribution = m_distribution;
        other->m_sampleCount = m_sampleCount;

        return std::move(other);
    }

    void IndependentSampler::prepare()
    {
        m_randomEngine.seed(std::random_device()());
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