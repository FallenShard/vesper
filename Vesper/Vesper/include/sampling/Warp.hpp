#pragma once

#include <math/Types.hpp>

namespace vesp
{
    class Warp
    {
    public:
        static Point2f squareToUniformSquare(const Point2f& sample);
        static float squareToUniformSquarePdf();

        static Point2f squareToUniformDisk(const Point2f& sample);
        static float squareToUniformDiskPdf();

        static Vector3f squareToUniformSphere(const Point2f& sample);
        static float squareToUniformSpherePdf();

        static Vector3f squareToUniformHemisphere(const Point2f& sample);
        static float squareToUniformHemispherePdf(const Vector3f& v);

        static Vector3f squareToUniformSphereCap(const Point2f& sample, float cosThetaMax);
        static float squareToUniformSphereCapPdf(const Vector3f& v, float cosThetaMax);

        static Vector3f squareToCosineHemisphere(const Point2f& sample);
        static float squareToCosineHemispherePdf(const Vector3f& v);

        static Vector3f squareToBeckmann(const Point2f& sample, float alpha);
        static float squareToBeckmannPdf(const Vector3f& v, float alpha);

        static Vector3f squareToUniformTriangle(const Point2f& sample);

        static Vector3f squareToUniformCylinder(const Point2f& sample, float cosThetaMin, float cosThetaMax);

    private:
        static inline Vector3f cylinderToSphereSection(const Point2f& sample, float cosThetaMin, float cosThetaMax);

    };
}