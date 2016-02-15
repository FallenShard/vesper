#pragma once

#include <math/Types.hpp>

namespace vesp
{
    class Sampler
    {
    public:
        Sampler();
        virtual ~Sampler();

        virtual void generate() = 0;
        virtual void advance() = 0;
        
        virtual float next1D() = 0;
        virtual Point2f next2D() = 0;

        virtual size_t getSampleCount() const;

    protected:
        size_t m_sampleCount;

    };
}
