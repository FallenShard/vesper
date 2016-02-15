#pragma once

#include <string>

namespace vesp
{
    class Scene;

    class FileParser
    {
    public:
        Scene* loadFromXml(const std::string& filename);
    };
}