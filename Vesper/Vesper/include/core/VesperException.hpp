#pragma once

#include <exception>
#include <string>

namespace vesp
{
    class VesperException : public std::exception
    {
    public:
        VesperException(const std::string& message);
        ~VesperException();

        std::string getMessage() const;

    private:
        std::string m_message;
    };

    std::ostream& operator<<(std::ostream& stream, const VesperException& ex);
}