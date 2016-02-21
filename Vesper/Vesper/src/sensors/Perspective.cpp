#include <sensors/Perspective.hpp>
#include <iostream>

namespace vesp
{
    PerspectiveSensor::PerspectiveSensor(const AttributeList& attributes)
    {
        m_imageSize.x() = attributes.getInteger("imageWidth", 800);
        m_imageSize.y() = attributes.getInteger("imageHeight", 600);
        m_fov = attributes.getFloat("fov", 45.f);
        m_nearClip = attributes.getFloat("nearClip", 1e-4f);
        m_farClip = attributes.getFloat("farClip", 1e4f);

        m_invImageSize = m_imageSize.cast<float>().cwiseInverse();

        m_cameraToWorld = Transform(Eigen::Affine3f(Eigen::Translation3f(0.f, 0.f, 0.f)).matrix());

        //config
        float aspect = m_imageSize.x() / static_cast<float>(m_imageSize.y());

        float recip = 1.f / (m_farClip - m_nearClip);
        float cot = 1.f / std::tanf(degToRad(m_fov / 2.f));

        Eigen::Matrix4f perspective;
        perspective <<
            cot, 0, 0, 0,
            0, cot, 0, 0,
            0, 0, m_farClip * recip, -m_nearClip * m_farClip * recip,
            0, 0, 1, 0;

        Eigen::DiagonalMatrix<float, 3> scale(Vector3f(0.5f, 0.5f * aspect, 1.f));
        Eigen::Translation<float, 3> shift(1.f, 1.f / aspect, 0.f);

        m_sampleToCamera = Transform(scale * shift * perspective).invert();
    }

    Spectrum PerspectiveSensor::sampleRay(Ray3f& ray, const Point2f& samplePosition, const Point2f& apertureSample) const
    {
        Point3f nearPoint = m_sampleToCamera * Point3f(
            samplePosition.x() * m_invImageSize.x(),
            samplePosition.y() * m_invImageSize.y(), 0.f);

        Vector3f dir = nearPoint.normalized();
        float invZ = 1.f / dir.z();

        ray.o = m_cameraToWorld * Point3f(0.f, 0.f, 0.f);
        ray.d = m_cameraToWorld * dir;
        ray.minT = m_nearClip * invZ;
        ray.maxT = m_farClip * invZ;
        ray.update();

        return Spectrum(1.f);
    }
}