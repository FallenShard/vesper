#include <fstream>
#include <iostream>

#include <rapidxml/rapidxml.hpp>

#include <io/FileParser.hpp>

#include <core/Scene.hpp>
#include <core/Utils.hpp>
#include <core/VesperException.hpp>
#include <core/VesperObjectFactory.hpp>

#include <math/Operations.hpp>

namespace vesp
{
    std::shared_ptr<Scene> FileParser::loadFromXml(const std::string& filename)
    {
        // Internal check in factory makes sure this function will only effectively run the function once! FIX!!
        // DO NOT ASSUME ABOUT IMPLEMENTATION OF STUFF
        VesperObjectFactory::initialize();

        // Load the scene file
        std::ifstream sceneFile(filename);
        std::stringstream buffer;
        buffer << sceneFile.rdbuf();
        sceneFile.close();
        std::string content(buffer.str());

        rapidxml::xml_document<> doc;
        doc.parse<0>(&content[0]);
        rapidxml::xml_node<>* root;

        // Make sure there's a scene node on the root level
        root = doc.first_node("scene");
        if (!root)
            throw VesperException("Invalid root note: root must be a scene tag!");

        // Primitive attribute types for nodes
        enum class AttributeType
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

        std::map<std::string, AttributeType> attributeTypes;
        attributeTypes["boolean"]   = AttributeType::Boolean;
        attributeTypes["integer"]   = AttributeType::Integer;
        attributeTypes["float"]     = AttributeType::Float;
        attributeTypes["string"]    = AttributeType::String;
        attributeTypes["spectrum"]  = AttributeType::Spectrum;
        attributeTypes["point2"]    = AttributeType::Point2f;
        attributeTypes["point3"]    = AttributeType::Point3f;
        attributeTypes["vector2"]   = AttributeType::Vector2f;
        attributeTypes["vector3"]   = AttributeType::Vector3f;
        attributeTypes["transform"] = AttributeType::Transform;

        // Valid node names for composite objects
        std::map<std::string, VesperObject::ClassType> classTypes;
        classTypes["sampler"]               = VesperObject::ClassType::Sampler;
        classTypes["integrator"]            = VesperObject::ClassType::Integrator;
        classTypes["sensor"]                = VesperObject::ClassType::Sensor;
        classTypes["reconstruction-filter"] = VesperObject::ClassType::ReconstructionFilter;
        classTypes["postprocess-filter"]    = VesperObject::ClassType::PostProcessFilter;
        classTypes["shape"]                 = VesperObject::ClassType::Shape;
        classTypes["emitter"]               = VesperObject::ClassType::Emitter;
        classTypes["medium"]                = VesperObject::ClassType::Medium;
        classTypes["bsdf"]                  = VesperObject::ClassType::BSDF;
        classTypes["phase-function"]        = VesperObject::ClassType::PhaseFunction;
        classTypes["subsurface"]            = VesperObject::ClassType::Subsurface;
        classTypes["texture"]               = VesperObject::ClassType::Texture;
        classTypes["volume"]                = VesperObject::ClassType::Volume;

        // Create the scene
        std::shared_ptr<Scene> scene = std::make_shared<Scene>();

        // This lambda recursively parses scene's children in the xml DOM
        std::function<std::shared_ptr<VesperObject>(rapidxml::xml_node<>*, int)> visit = [&, this](rapidxml::xml_node<>* node, int depth)
        {
            // Create an attribute list for the current node
            AttributeList attribList;

            // Make sure there's a type on the first level node in inside the scene
            auto typeAttrib = node->first_attribute("type");
            if (!typeAttrib)
                throw VesperException("Composite node wasn't given a type!");

            std::string typeStr = typeAttrib->value();

            // Gather children in post-order inside this vector
            std::vector<std::shared_ptr<VesperObject>> children;
            
            // Parse the children of the current node
            for (auto* child = node->first_node(); child; child = child->next_sibling())
            {
                std::string childName(child->name());

                // Attempt to parse the child as attribute ("primitive") type
                if (attributeTypes.find(childName) != attributeTypes.end())
                {
                    auto attribType = attributeTypes.find(childName)->second;
                    auto xmlValueAttrib = child->first_attribute("value");
                    auto xmlNameAttrib = child->first_attribute("name");

                    if (!xmlNameAttrib)
                        throw VesperException("Primitive attribute is missing a name!");

                    std::string nameStr = xmlNameAttrib->value();

                    std::string valueStr;
                    if (attribType != AttributeType::Transform)
                    {
                        if (!xmlValueAttrib)
                            throw VesperException("Primitive attribute " + nameStr + " is missing a value!");
                        else
                            valueStr = std::string(xmlValueAttrib->value());
                    }

                    switch (attribType)
                    {
                    case AttributeType::Boolean:
                    {
                        bool val = !strcmp(xmlValueAttrib->value(), "true");
                        attribList.setBoolean(nameStr, val);
                        break;
                    }
                    case AttributeType::Integer:
                    {
                        int val = std::stoi(valueStr);
                        attribList.setInteger(nameStr, val);
                        break;
                    }
                    case AttributeType::Float:
                    {
                        float val = std::stof(valueStr);
                        attribList.setFloat(nameStr, val);
                        break;
                    }
                    case AttributeType::String:
                    {
                        attribList.setString(nameStr, valueStr);
                        break;
                    }
                    case AttributeType::Spectrum:
                    {
                        Spectrum val = parseFloat3<Spectrum>(valueStr);
                        attribList.setSpectrum(nameStr, val);
                        break;
                    }
                    case AttributeType::Point2f:
                    {
                        Point2f val = parseFloat2<Point2f>(valueStr);
                        attribList.setPoint2f(nameStr, val);
                        break;
                    }
                    case AttributeType::Point3f:
                    {
                        Point3f val = parseFloat3<Point3f>(valueStr);
                        attribList.setPoint3f(nameStr, val);
                        break;
                    }
                    case AttributeType::Vector2f:
                    {
                        Vector2f val = parseFloat2<Vector2f>(valueStr);
                        attribList.setVector2f(nameStr, val);
                        break;
                    }
                    case AttributeType::Vector3f:
                    {
                        Vector3f val = parseFloat3<Vector3f>(valueStr);
                        attribList.setVector3f(nameStr, val);
                        break;
                    }
                    case AttributeType::Transform:
                    {
                        Transform transform;

                        for (auto* transChild = child->first_node(); transChild; transChild = transChild->next_sibling())
                        {
                            std::string transformStr = transChild->name();

                            if (transformStr == "translation")
                            {
                                auto translationAttrib = transChild->first_attribute("value");
                                if (!translationAttrib)
                                    throw VesperException("Translation had no value specified!");
                                std::string translationStr = translationAttrib->value();
                                Vector3f translationVec = parseFloat3<Vector3f>(translationStr);
                                transform.translate(translationVec);
                            }
                            else if (transformStr == "scale")
                            {
                                auto scaleAttrib = transChild->first_attribute("value");
                                if (!scaleAttrib)
                                    throw VesperException("Scale had no value specified!");
                                std::string scaleStr = scaleAttrib->value();
                                Vector3f scaleVec = parseFloat3<Vector3f>(scaleStr);
                                transform.scale(scaleVec);
                            }
                            else if (transformStr == "rotation")
                            {
                                auto axisAttrib = transChild->first_attribute("axis");
                                auto angleAttrib = transChild->first_attribute("angle");
                                if (!axisAttrib)
                                    throw VesperException("Rotation had no axis specified!");
                                if (!angleAttrib)
                                    throw VesperException("Rotation had no angle specified!");

                                std::string axisStr = axisAttrib->value();
                                std::string angleStr = angleAttrib->value();
                                Vector3f axisVec = parseFloat3<Vector3f>(axisStr);
                                transform.rotate(degToRad(std::stof(angleStr)), axisVec.normalized());
                            }
                        }

                        attribList.setTransform(nameStr, transform);
                        break;
                    }
                    }
                }
                else if (classTypes.find(childName) != classTypes.end()) // Attempt to parse as composite node
                {
                    children.emplace_back(visit(child, depth + 1)); // Just recurse and keep the result in the vector
                }
                else // Invalid node, xml uses unknown node names
                {
                    throw VesperException("Unknown xml node encountered: " + childName);
                }
            }

            // Create an object out of accumulated attributes and type in the string
            auto vespObject = VesperObjectFactory::create(typeStr, attribList);

            // Add its children
            for (auto& child : children)
            {
                vespObject->addChild(child);
                child->setParent(vespObject);
            }

            // Call the configure callback (all children are initialized) and return
            vespObject->setClassType(classTypes[node->name()]);
            vespObject->configure();
            return vespObject;
        };

        // This is where the parsing essentially begins, add first-level nodes inside the scene
        for (auto* child = root->first_node(); child; child = child->next_sibling())
            scene->addChild(visit(child, 1));

        // Free the memory of the file
        doc.clear();

        // Configure the scene and finally return
        scene->configure();
        return scene;
    }
}