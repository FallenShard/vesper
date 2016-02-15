#pragma once

#include <eigen/Dense>

#include <cameras/Camera.hpp>
#include <math/Transform.hpp>
#include <math/Operations.hpp>

namespace vesp
{
    class PerspectiveCamera : public Camera
    {
    public:
        PerspectiveCamera(int width, int height);

        virtual Spectrum sampleRay(Ray3f& ray, const Point2f& samplePosition, const Point2f& apertureSample) const;

    private:
        Vector2f m_invImageSize;
        Transform m_sampleToCamera;
        Transform m_cameraToWorld;
        float m_fov;
        float m_nearClip;
        float m_farClip;
    };
}