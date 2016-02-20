#include <shapes/Shape.hpp>

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
        }
    }

    const BSDF* Shape::getBSDF() const
    {
        return m_bsdf.get();
    }
}