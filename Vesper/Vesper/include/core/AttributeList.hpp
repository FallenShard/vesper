#pragma once

#include <map>

#include <math/Types.hpp>
#include <math/Transform.hpp>
#include <spectrum/Spectrum.hpp>

namespace vesp
{
    class AttributeList
    {
    public:
        AttributeList();
        ~AttributeList();

        bool contains(const std::string& name) const;

        void setBoolean(const std::string& name, const bool value);
        bool getBoolean(const std::string& name) const;
        bool getBoolean(const std::string& name, const bool defValue) const;

        void setInteger(const std::string& name, const int value);
        int getInteger(const std::string& name) const;
        int getInteger(const std::string& name, const int defValue) const;

        void setFloat(const std::string& name, const float value);
        float getFloat(const std::string& name) const;
        float getFloat(const std::string& name, const float defValue) const;

        void setString(const std::string& name, const std::string& value);
        std::string getString(const std::string& name) const;
        std::string getString(const std::string& name, const std::string& defValue) const;

        void setSpectrum(const std::string& name, const Spectrum& value);
        Spectrum getSpectrum(const std::string& name) const;
        Spectrum getSpectrum(const std::string& name, const Spectrum& defValue) const;

        void setPoint2f(const std::string& name, const Point2f& value);
        Point2f getPoint2f(const std::string& name) const;
        Point2f getPoint2f(const std::string& name, const Point2f& defValue) const;

        void setPoint3f(const std::string& name, const Point3f& value);
        Point3f getPoint3f(const std::string& name) const;
        Point3f getPoint3f(const std::string& name, const Point3f& defValue) const;

        void setVector2f(const std::string& name, const Vector2f& value);
        Vector2f getVector2f(const std::string& name) const;
        Vector2f getVector2f(const std::string& name, const Vector2f& defValue) const;

        void setVector3f(const std::string& name, const Vector3f& value);
        Vector3f getVector3f(const std::string& name) const;
        Vector3f getVector3f(const std::string& name, const Vector3f& defValue) const;

        void setTransform(const std::string& name, const Transform& value);
        Transform getTransform(const std::string& name) const;
        Transform getTransform(const std::string& name, const Transform& defValue) const;

    private:
        enum class Type
        {
            Boolean,
            Integer,
            Float,
            String,
            Spectrum,
            Point2f,
            Point3f,
            Vector2f,
            Vector3f,
            Transform
        };

        // Should totally implement unrestricted union here to optimize for memory
        struct Value
        {
            bool booleanValue;
            int integerValue;
            float floatValue;
            std::string stringValue;
            Spectrum spectrumValue;
            Point2f point2fValue;
            Point3f point3fValue;
            Vector2f vector2fValue;
            Vector3f vector3fValue;
            Transform transformValue;

            Value() {}
            ~Value() {}
        };

        struct Attribute
        {
            Type type;
            Value value;
        };

        std::map<std::string, Attribute> m_attributes;
    };
}