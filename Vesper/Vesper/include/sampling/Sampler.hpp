#pragma once

#include <math/Types.hpp>
#include <core/VesperObject.hpp>

namespace vesp
{
    class Sampler : public VesperObject
    {
    public:
        Sampler();
        virtual ~Sampler();

        virtual std::unique_ptr<Sampler> clone() const = 0;

        virtual void prepare() = 0;
        virtual void generate() = 0;
        virtual void advance() = 0;
        
        virtual float next1D() = 0;
        virtual Point2f next2D() = 0;

        virtual size_t getSampleCount() const;

        virtual ClassType getClassType() const override { return ClassType::Sampler; }

    protected:
        size_t m_sampleCount;

    };
}
