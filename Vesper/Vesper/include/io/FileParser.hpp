#pragma once

#include <string>
#include <memory>

namespace vesp
{
    class Scene;

    class FileParser
    {
    public:
        std::shared_ptr<Scene> loadFromXml(const std::string& filename);

    private:
        template <typename T>
        T parseFloat3(const std::string& str) const;

        template <typename T>
        T parseFloat2(const std::string& str) const;
    };

    template<typename T>
    inline T FileParser::parseFloat3(const std::string& str) const
    {
        auto tokens = Utils::tokenize(str, " ");

        float array[3] = { 0.f };
        for (int i = 0; i < 3; i++)
            if (tokens.size() > i) array[i] = static_cast<float>(std::stof(tokens[i]));

        return T(array[0], array[1], array[2]);
    }

    template<typename T>
    inline T FileParser::parseFloat2(const std::string & str) const
    {
        auto tokens = Utils::tokenize(str, " ");

        float array[2] = { 0.f };
        for (int i = 0; i < 2; i++)
            if (tokens.size() > i) array[i] = static_cast<float>(std::stof(tokens[i]));

        return T(array[0], array[1]);
    }
}