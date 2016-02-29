#include <sensors/Sensor.hpp>
#include <reconstruction/ReconstructionFilter.hpp>

namespace vesp
{
    void Sensor::addChild(std::shared_ptr<VesperObject> child)
    {
        switch (child->getClassType())
        {
        case ClassType::ReconstructionFilter:
        {
            m_filter = std::static_pointer_cast<ReconstructionFilter>(child);
            break;
        }
        }
    }

    const Vector2i& Sensor::getImageSize() const
    {
        return m_imageSize;
    }
    const ReconstructionFilter* Sensor::getReconstructionFilter() const
    {
        return m_filter.get();
    }
}