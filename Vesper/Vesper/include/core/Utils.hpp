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
    };
}