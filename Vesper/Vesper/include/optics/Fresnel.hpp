#pragma once

#include <algorithm>

namespace vesp
{
    class Fresnel
    {
    public:
        inline static float dielectric(float cosThetaI, float extIOR, float intIOR)
        {
            float etaI = extIOR, etaT = intIOR;

            // If indices of refraction are the same, no fresnel effects
            if (extIOR == intIOR)
                return 0.f;

            // if cosThetaI is < 0.f, it means the ray is coming from inside the object
            if (cosThetaI < 0.f)
            {
                std::swap(etaI, etaT);
                cosThetaI = -cosThetaI;
            }

            float eta = etaI / etaT;
            float sinThetaTSqr = eta * eta * (1.f - cosThetaI * cosThetaI);

            // Total internal reflection
            if (sinThetaTSqr > 1.f)
                return 1.f;

            float cosThetaT = std::sqrtf(1.f - sinThetaTSqr);

            float Rs = (etaI * cosThetaI - etaT * cosThetaT) / (etaI * cosThetaI + etaT * cosThetaT);
            float Rp = (etaT * cosThetaI - etaI * cosThetaT) / (etaT * cosThetaI + etaI * cosThetaT);
            return (Rs * Rs + Rp * Rp) / 2.f;
        }
    };
}