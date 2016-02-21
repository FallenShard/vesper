#include <shapes/Shape.hpp>
#include <bsdfs/BSDF.hpp>
#include <emitters/Emitter.hpp>

namespace vesp
{
    void Shape::addChild(std::shared_ptr<VesperObject> child)
    {
        switch (child->getClassType())
        {
        case ClassType::BSDF:
        {
            m_bsdf = std::static_pointer_cast<BSDF>(child);
            break;
        }

        case ClassType::Emitter:
        {
            m_emitter = std::static_pointer_cast<Emitter>(child);
            break;
        }
        }
    }

    const BSDF* Shape::getBSDF() const
    {
        return m_bsdf.get();
    }
    const Emitter * Shape::getEmitter() const
    {
        return m_emitter.get();
    }
    std::shared_ptr<Emitter> Shape::getEmitterManaged() const
    {
        return m_emitter;
    }
}