#pragma once

#include <random>

#include <sampling/Sampler.hpp>

namespace vesp
{
    class IndependentSampler : public Sampler
    {
    public:
        IndependentSampler();
        ~IndependentSampler();

        virtual void generate();
        virtual void advance();

        virtual float next1D();
        virtual Point2f next2D();

    private:
        inline float nextFloat();

        std::default_random_engine m_randomEngine;
        std::uniform_real_distribution<float> m_distribution;
    };
}