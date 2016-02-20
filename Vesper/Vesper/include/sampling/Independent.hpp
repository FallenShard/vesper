#pragma once

#include <random>

#include <core/AttributeList.hpp>
#include <sampling/Sampler.hpp>

namespace vesp
{
    class IndependentSampler : public Sampler
    {
    public:
        IndependentSampler(const AttributeList& attributes);
        IndependentSampler();
        ~IndependentSampler();

        virtual std::unique_ptr<Sampler> clone() const override;

        virtual void generate() override;
        virtual void advance() override;

        virtual float next1D() override;
        virtual Point2f next2D() override;

    private:
        inline float nextFloat();

        std::default_random_engine m_randomEngine;
        std::uniform_real_distribution<float> m_distribution;
    };
}