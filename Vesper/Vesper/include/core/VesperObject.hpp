#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <map>

#include <core/AttributeList.hpp>

namespace vesp
{
    class VesperObject
    {
    public:
        enum class ClassType
        {
            Scene,

            Sampler,
            Integrator,

            Sensor,
            ReconstructionFilter,
            PostProcessFilter,

            Shape,
            Emitter,
            Medium,
            
            BSDF,
            PhaseFunction,
            Subsurface,

            Texture,
            Volume
        };

        virtual ~VesperObject() = 0;

        virtual void addChild(std::shared_ptr<VesperObject> child);
        virtual void configure();
        virtual void setParent(std::weak_ptr<VesperObject> parent);

        void setClassType(ClassType classType);
        ClassType getClassType() const;

    private:
        ClassType m_classType;
    };
}