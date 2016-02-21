#include <core/VesperObjectFactory.hpp>
#include <core/VesperException.hpp>

#include <sampling/Independent.hpp>

#include <integrators/DirectLighting.hpp>

#include <sensors/Perspective.hpp>

#include <shapes/WavefrontObjMesh.hpp>

#include <bsdfs/Lambertian.hpp>

#include <emitters/PointLight.hpp>


namespace vesp
{
    std::map<std::string, VesperObjectFactory::ConstructorFunc> VesperObjectFactory::ConstructorMap;

    void VesperObjectFactory::initialize()
    {
        static bool initialized = false;
        if (!initialized)
        {
            std::cout << "Initializing object factory... " << std::endl;

            registerClass<IndependentSampler>("independent");

            registerClass<DirectLightingIntegrator>("direct-lighting");

            registerClass<PerspectiveSensor>("perspective");
        
            registerClass<WavefrontObjMesh>("objMesh");

            registerClass<LambertianBSDF>("lambertian");

            registerClass<PointLight>("point");

            initialized = true;
        }
    }

    bool VesperObjectFactory::isRegistered(const std::string& name)
    {
        return ConstructorMap.find(name) != ConstructorMap.end();
    }

    std::shared_ptr<VesperObject> VesperObjectFactory::create(const std::string& className, const AttributeList& attributes)
    {
        auto constructor = ConstructorMap.find(className);

        if (constructor == ConstructorMap.end())
            throw VesperException("Attempt to create a non-registered class: " + className);

        return std::shared_ptr<VesperObject>(constructor->second(attributes));
    }
}