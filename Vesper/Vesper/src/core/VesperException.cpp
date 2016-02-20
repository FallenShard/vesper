#include <iostream>
#include <core/VesperException.hpp>

namespace vesp
{
    VesperException::VesperException(const std::string& message)
        : m_message(message)
    {
    }

    VesperException::~VesperException()
    {
    }

    std::string VesperException::getMessage() const
    {
        return m_message;
    }

    std::ostream& operator<<(std::ostream& out, const VesperException& vEx)
    {
        out << vEx.getMessage();
        return out;
    }
}