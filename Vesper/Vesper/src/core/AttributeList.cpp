#include <core/AttributeList.hpp>
#include <core/VesperException.hpp>

namespace vesp
{
    AttributeList::AttributeList()
    {
    }

    AttributeList::~AttributeList()
    {
    }

    bool AttributeList::contains(const std::string& name) const
    {
        return m_attributes.find(name) != m_attributes.end();
    }

    void AttributeList::setBoolean(const std::string &name, const bool value)
    {
        Attribute attrib;
        attrib.type = Type::Boolean;
        attrib.value.booleanValue = value;
        m_attributes.insert(std::make_pair(name, attrib));
    }

    bool AttributeList::getBoolean(const std::string& name) const
    {
        auto itemIter = m_attributes.find(name);
        if (itemIter == m_attributes.end())
            throw VesperException("Value with name: " + name + " could not be found!");
        if (itemIter->second.type != Type::Boolean)
            throw VesperException("Value with name: " + name + " is not of boolean type!");

        return itemIter->second.value.booleanValue;
    }

    bool AttributeList::getBoolean(const std::string& name, const bool defValue) const
    {
        auto itemIter = m_attributes.find(name);
        if (itemIter == m_attributes.end())
            return defValue;
        if (itemIter->second.type != Type::Boolean)
            throw VesperException("Value with name: " + name + " is not of boolean type!");

        return itemIter->second.value.booleanValue;
    }

    void AttributeList::setInteger(const std::string& name, const int value)
    {
        Attribute attrib;
        attrib.type = Type::Integer;
        attrib.value.integerValue = value;
        m_attributes.insert(std::make_pair(name, attrib));
    }

    int AttributeList::getInteger(const std::string& name) const
    {
        auto itemIter = m_attributes.find(name);
        if (itemIter == m_attributes.end())
            throw VesperException("Value with name: " + name + " could not be found!");
        if (itemIter->second.type != Type::Integer)
            throw VesperException("Value with name: " + name + " is not of integer type!");

        return itemIter->second.value.integerValue;
    }

    int AttributeList::getInteger(const std::string& name, const int defValue) const
    {
        auto itemIter = m_attributes.find(name);
        if (itemIter == m_attributes.end())
            return defValue;
        if (itemIter->second.type != Type::Integer)
            throw VesperException("Value with name: " + name + " is not of integer type!");

        return itemIter->second.value.integerValue;
    }

    void AttributeList::setFloat(const std::string& name, const float value)
    {
        Attribute attrib;
        attrib.type = Type::Float;
        attrib.value.floatValue = value;
        m_attributes.insert(std::make_pair(name, attrib));
    }

    float AttributeList::getFloat(const std::string& name) const
    {
        auto itemIter = m_attributes.find(name);
        if (itemIter == m_attributes.end())
            throw VesperException("Value with name: " + name + " could not be found!");
        if (itemIter->second.type != Type::Float)
            throw VesperException("Value with name: " + name + " is not of float type!");

        return itemIter->second.value.floatValue;
    }

    float AttributeList::getFloat(const std::string& name, const float defValue) const
    {
        auto itemIter = m_attributes.find(name);
        if (itemIter == m_attributes.end())
            return defValue;
        if (itemIter->second.type != Type::Float)
            throw VesperException("Value with name: " + name + " is not of float type!");

        return itemIter->second.value.floatValue;
    }

    void AttributeList::setString(const std::string& name, const std::string& value)
    {
        Attribute attrib;
        attrib.type = Type::String;
        attrib.value.stringValue = value;
        m_attributes.insert(std::make_pair(name, attrib));
    }

    std::string AttributeList::getString(const std::string& name) const
    {
        auto itemIter = m_attributes.find(name);
        if (itemIter == m_attributes.end())
            throw VesperException("Value with name: " + name + " could not be found!");
        if (itemIter->second.type != Type::String)
            throw VesperException("Value with name: " + name + " is not of string type!");

        return itemIter->second.value.stringValue;
    }

    std::string AttributeList::getString(const std::string& name, const std::string& defValue) const
    {
        auto itemIter = m_attributes.find(name);
        if (itemIter == m_attributes.end())
            return defValue;
        if (itemIter->second.type != Type::String)
            throw VesperException("Value with name: " + name + " is not of string type!");

        return itemIter->second.value.stringValue;
    }

    void AttributeList::setSpectrum(const std::string& name, const Spectrum& value)
    {
        Attribute attrib;
        attrib.type = Type::Spectrum;
        attrib.value.spectrumValue = value;
        m_attributes.insert(std::make_pair(name, attrib));
    }

    Spectrum AttributeList::getSpectrum(const std::string& name) const
    {
        auto itemIter = m_attributes.find(name);
        if (itemIter == m_attributes.end())
            throw VesperException("Value with name: " + name + " could not be found!");
        if (itemIter->second.type != Type::Spectrum)
            throw VesperException("Value with name: " + name + " is not of spectrum type!");

        return itemIter->second.value.spectrumValue;
    }

    Spectrum AttributeList::getSpectrum(const std::string& name, const Spectrum& defValue) const
    {
        auto itemIter = m_attributes.find(name);
        if (itemIter == m_attributes.end())
            return defValue;
        if (itemIter->second.type != Type::Spectrum)
            throw VesperException("Value with name: " + name + " is not of spectrum type!");

        return itemIter->second.value.spectrumValue;
    }

    void AttributeList::setPoint2f(const std::string& name, const Point2f& value)
    {
        Attribute attrib;
        attrib.type = Type::Point2f;
        attrib.value.point2fValue = value;
        m_attributes.insert(std::make_pair(name, attrib));
    }

    Point2f AttributeList::getPoint2f(const std::string& name) const
    {
        auto itemIter = m_attributes.find(name);
        if (itemIter == m_attributes.end())
            throw VesperException("Value with name: " + name + " could not be found!");
        if (itemIter->second.type != Type::Point2f)
            throw VesperException("Value with name: " + name + " is not of point2f type!");

        return itemIter->second.value.point2fValue;
    }

    Point2f AttributeList::getPoint2f(const std::string& name, const Point2f& defValue) const
    {
        auto itemIter = m_attributes.find(name);
        if (itemIter == m_attributes.end())
            return defValue;
        if (itemIter->second.type != Type::Point2f)
            throw VesperException("Value with name: " + name + " is not of point2f type!");

        return itemIter->second.value.point2fValue;
    }

    void AttributeList::setPoint3f(const std::string& name, const Point3f& value)
    {
        Attribute attrib;
        attrib.type = Type::Point3f;
        attrib.value.point3fValue = value;
        m_attributes.insert(std::make_pair(name, attrib));
    }

    Point3f AttributeList::getPoint3f(const std::string& name) const
    {
        auto itemIter = m_attributes.find(name);
        if (itemIter == m_attributes.end())
            throw VesperException("Value with name: " + name + " could not be found!");
        if (itemIter->second.type != Type::Point3f)
            throw VesperException("Value with name: " + name + " is not of point3f type!");

        return itemIter->second.value.point3fValue;
    }

    Point3f AttributeList::getPoint3f(const std::string& name, const Point3f& defValue) const
    {
        auto itemIter = m_attributes.find(name);
        if (itemIter == m_attributes.end())
            return defValue;
        if (itemIter->second.type != Type::Point3f)
            throw VesperException("Value with name: " + name + " is not of point3f type!");

        return itemIter->second.value.point3fValue;
    }

    void AttributeList::setVector2f(const std::string& name, const Vector2f& value)
    {
        Attribute attrib;
        attrib.type = Type::Vector2f;
        attrib.value.vector2fValue = value;
        m_attributes.insert(std::make_pair(name, attrib));
    }

    Vector2f AttributeList::getVector2f(const std::string& name) const
    {
        auto itemIter = m_attributes.find(name);
        if (itemIter == m_attributes.end())
            throw VesperException("Value with name: " + name + " could not be found!");
        if (itemIter->second.type != Type::Vector2f)
            throw VesperException("Value with name: " + name + " is not of vector2f type!");

        return itemIter->second.value.vector2fValue;
    }

    Vector2f AttributeList::getVector2f(const std::string& name, const Vector2f& defValue) const
    {
        auto itemIter = m_attributes.find(name);
        if (itemIter == m_attributes.end())
            return defValue;
        if (itemIter->second.type != Type::Vector2f)
            throw VesperException("Value with name: " + name + " is not of vector2f type!");

        return itemIter->second.value.vector2fValue;
    }

    void AttributeList::setVector3f(const std::string& name, const Vector3f& value)
    {
        Attribute attrib;
        attrib.type = Type::Vector3f;
        attrib.value.vector3fValue = value;
        m_attributes.insert(std::make_pair(name, attrib));
    }

    Vector3f AttributeList::getVector3f(const std::string& name) const
    {
        auto itemIter = m_attributes.find(name);
        if (itemIter == m_attributes.end())
            throw VesperException("Value with name: " + name + " could not be found!");
        if (itemIter->second.type != Type::Vector3f)
            throw VesperException("Value with name: " + name + " is not of vector3f type!");

        return itemIter->second.value.vector3fValue;
    }

    Vector3f AttributeList::getVector3f(const std::string& name, const Vector3f& defValue) const
    {
        auto itemIter = m_attributes.find(name);
        if (itemIter == m_attributes.end())
            return defValue;
        if (itemIter->second.type != Type::Vector3f)
            throw VesperException("Value with name: " + name + " is not of vector3f type!");

        return itemIter->second.value.vector3fValue;
    }

    void AttributeList::setTransform(const std::string& name, const Transform& value)
    {
        Attribute attrib;
        attrib.type = Type::Transform;
        attrib.value.transformValue = value;
        m_attributes.insert(std::make_pair(name, attrib));
    }

    Transform AttributeList::getTransform(const std::string& name) const
    {
        auto itemIter = m_attributes.find(name);
        if (itemIter == m_attributes.end())
            throw VesperException("Value with name: " + name + " could not be found!");
        if (itemIter->second.type != Type::Transform)
            throw VesperException("Value with name: " + name + " is not of transform type!");

        return itemIter->second.value.transformValue;
    }

    Transform AttributeList::getTransform(const std::string& name, const Transform& defValue) const
    {
        auto itemIter = m_attributes.find(name);
        if (itemIter == m_attributes.end())
            return defValue;
        if (itemIter->second.type != Type::Transform)
            throw VesperException("Value with name: " + name + " is not of transform type!");

        return itemIter->second.value.transformValue;
    }
}