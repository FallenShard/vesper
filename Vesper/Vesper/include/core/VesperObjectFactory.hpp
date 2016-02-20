#pragma once

#include <memory>
#include <functional>

#include <core/VesperObject.hpp>
#include <core/AttributeList.hpp>

namespace vesp
{
    class VesperObjectFactory
    {
    public:
        using ConstructorFunc = std::function<VesperObject*(const AttributeList&)>;

        static void initialize();

        template <typename T>
        static void registerClass(const std::string& className)
        {
            ConstructorMap[className] = [](const AttributeList& attributes)
            {
                return new T(attributes);
            };
        }
        static bool isRegistered(const std::string& name);

        static std::shared_ptr<VesperObject> create(const std::string& className, const AttributeList& attributes);

        static std::map<std::string, ConstructorFunc> ConstructorMap;
    };
}
