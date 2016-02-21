#include <sampling/Warp.hpp>
#include <math/Operations.hpp>

namespace vesp
{
    Point2f Warp::squareToUniformSquare(const Point2f& sample)
    {
        return sample;
    }

    float Warp::squareToUniformSquarePdf()
    {
        return 1.f;
    }

    Point2f Warp::squareToUniformDisk(const Point2f& sample)
    {
        float r = sqrtf(sample.y());
        float theta = 2 * PI * sample.x();
        return Point2f(r * cosf(theta), r * sinf(theta));
    }

    float Warp::squareToUniformDiskPdf()
    {
        return InvPI;
    }

    Vector3f Warp::squareToUniformSphere(const Point2f& sample)
    {
        return cylinderToSphereSection(sample, 1.f, -1.f);
    }

    float Warp::squareToUniformSpherePdf()
    {
        return InvFourPI;
    }
    Vector3f Warp::squareToUniformHemisphere(const Point2f& sample)
    {
        return cylinderToSphereSection(sample, 1.f, 0.f);
    }
    float Warp::squareToUniformHemispherePdf(const Vector3f& v)
    {
        return InvTwoPI;
    }
    Vector3f Warp::squareToUniformSphereCap(const Point2f& sample, float cosThetaMax)
    {
        return cylinderToSphereSection(sample, 1.f, cosThetaMax);
    }

    float Warp::squareToUniformSphereCapPdf(const Vector3f& v, float cosThetaMax)
    {
        return InvTwoPI / (1.f - cosThetaMax);
    }

    Vector3f Warp::squareToCosineHemisphere(const Point2f& sample)
    {
        float radius = sqrtf(sample.y());
        float theta = 2 * PI * sample.x();
        float x = radius * cosf(theta);
        float y = radius * sinf(theta);
        return Vector3f(x, y, 1 - x * x - y * y);
    }

    float Warp::squareToCosineHemispherePdf(const Vector3f& v)
    {
        return v.z() * InvPI;
    }

    Vector3f Warp::squareToBeckmann(const Point2f &sample, float alpha)
    {
        float denominator = 1 - alpha * alpha * log(1 - sample.y());
        float cosTheta = sqrtf(fabs(1 / denominator));
        float theta = acosf(cosTheta);
        float phi = 2 * PI * sample.x();
        float sinTheta = sinf(theta);
        
        return Vector3f(sinTheta * cosf(phi), sinTheta * sinf(phi), cosTheta);
    }

    float Warp::squareToBeckmannPdf(const Vector3f &m, float alpha)
    {
        float cosTheta2 = m.z() * m.z();
        float alpha2 = alpha * alpha;
        float numerator = exp((cosTheta2 - 1.f) / (cosTheta2 * alpha2));
        float denominator = PI * alpha2 * cosTheta2 * m.z();

        return numerator / denominator;
    }

    Vector3f Warp::squareToUniformTriangle(const Point2f& sample)
    {
        float val = sqrtf(sample.x());
        float u = 1.f - val;
        float v = sample.y() * val;
        return Vector3f(u, v, 1.f - u - v);
    }

    Vector3f Warp::squareToUniformCylinder(const Point2f& sample, float cosThetaMin, float cosThetaMax)
    {
        float z = cosThetaMin + sample.y() * (cosThetaMax - cosThetaMin);
        float phi = 2 * PI * sample.x();

        return Vector3f(cosf(phi), sinf(phi), z);
    }

    inline Vector3f Warp::cylinderToSphereSection(const Point2f& sample, float cosThetaMin, float cosThetaMax)
    {
        Vector3f cylinderPt = squareToUniformCylinder(sample, cosThetaMin, cosThetaMax);
        float radius = sqrtf(1.f - cylinderPt.z() * cylinderPt.z());
        return Vector3f(cylinderPt.x() * radius, cylinderPt.y() * radius, cylinderPt.z());
    }
}