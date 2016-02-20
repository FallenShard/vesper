#include <core/VesperObject.hpp>

namespace vesp
{
    VesperObject::~VesperObject()
    {
    }

    void VesperObject::addChild(std::shared_ptr<VesperObject> child)
    {
    }

    void VesperObject::configure()
    {
    }

    void VesperObject::setParent(std::weak_ptr<VesperObject> parent)
    {
    }

    void VesperObject::setClassType(VesperObject::ClassType classType)
    {
        m_classType = classType;
    }

    VesperObject::ClassType VesperObject::getClassType() const
    {
        return m_classType;
    }
}