#pragma once

#include <string>
#include <iostream>
#include <fstream>

namespace vesp
{
    class Utils
    {
    public:
        static std::string fileToString(const std::string& fileName)
        {
            std::ifstream inputFile(fileName);
            std::string source;

            if (inputFile.is_open())
            {
                std::string line;
                while (std::getline(inputFile, line))
                {
                    source += line + '\n';
                }
                inputFile.close();
            }
            else
                std::cout << "Could not open file " << fileName << std::endl;

            return source;
        }

        static std::vector<std::string> tokenize(const std::string& string, const std::string& delimiter)
        {
            std::vector<std::string> result;
            size_t start = 0;
            size_t end = 0;

            while (end != std::string::npos)
            {
                end = string.find(delimiter, start);

                result.push_back(string.substr(start, (end == std::string::npos) ? std::string::npos : end - start));

                start = ((end > (std::string::npos - delimiter.size())) ? std::string::npos : end + delimiter.size());
            }

            return result;
        }
    };
}