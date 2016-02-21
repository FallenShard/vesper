#pragma once

#include <eigen/Dense>

#include <sensors/Sensor.hpp>
#include <math/Transform.hpp>
#include <math/Operations.hpp>

namespace vesp
{
    class PerspectiveSensor : public Sensor
    {
    public:
        PerspectiveSensor(const AttributeList& attributes);

        virtual void configure() override;

        virtual Spectrum sampleRay(Ray3f& ray, const Point2f& samplePosition, const Point2f& apertureSample) const;

    private:
        Vector2f m_invImageSize;

        float m_fov;
        float m_nearClip;
        float m_farClip;

        Transform m_sampleToCamera;
        Transform m_cameraToWorld;
    };
}