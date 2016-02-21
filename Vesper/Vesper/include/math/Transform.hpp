#pragma once

#include <math/Types.hpp>
#include <math/Operations.hpp>

namespace vesp
{
    struct Transform
    {
        Eigen::Matrix4f transform;
        Eigen::Matrix4f inverse;

        Transform()
            : transform(Eigen::Matrix4f::Identity())
            , inverse(Eigen::Matrix4f::Identity())
        {
        }

        Transform(const Eigen::Matrix4f& trans)
            : transform(trans)
            , inverse(trans.inverse())
        {
        }

        Transform(const Eigen::Matrix4f& trans, const Eigen::Matrix4f& inv)
            : transform(trans)
            , inverse(inv)
        {
        }

        const Eigen::Matrix4f& getMatrix() const
        {
            return transform;
        }

        const Eigen::Matrix4f& getInverse() const
        {
            return inverse;
        }

        Transform invert() const
        {
            return Transform(inverse, transform);
        }

        Transform operator*(const Transform& t) const
        {
            return Transform(transform * t.transform, t.inverse * inverse);
        }

        Vector3f operator*(const Vector3f& v) const
        {
            return transform.topLeftCorner<3, 3>() * v;
        }
        
        Normal3f operator*(const Normal3f& n) const
        {
            return inverse.topLeftCorner<3, 3>().transpose() * n;
        }
        
        Point3f operator*(const Point3f& p) const
        {
            Vector4f homog = transform * Vector4f(p[0], p[1], p[2], 1.f);
            return homog.head<3>() / homog.w();
        }
        
        Ray3f operator*(const Ray3f& ray) const
        {
            return Ray3f(operator*(ray.o), operator*(ray.d), ray.minT, ray.maxT);
        }

        Transform& translate(const Vector3f& v)
        {
            Transform trans(Eigen::Affine3f(Eigen::Translation3f(v)).matrix());
            *this = trans * *this;
            return *this;
        }

        Transform& scale(const Vector3f& v)
        {
            Transform sc(Eigen::Affine3f(Eigen::DiagonalMatrix<float, 3>(v)).matrix());
            *this = sc * *this;
            return *this;
        }

        Transform& rotate(float angle, const Vector3f& axis)
        {
            Transform rot(Eigen::Affine3f(Eigen::AngleAxis<float>(angle, axis)).matrix());
            *this = rot * *this;
            return *this;
        }

        static Transform createTranslation(const Vector3f& v)
        {
            return Eigen::Affine3f(Eigen::Translation3f(v)).matrix();
        }

        static Transform createScale(const Vector3f& v)
        {
            return Eigen::Affine3f(Eigen::DiagonalMatrix<float, 3>(v)).matrix();
        }

        static Transform createRotation(float angle, const Vector3f& axis)
        {
            return Eigen::Affine3f(Eigen::AngleAxis<float>(angle, axis)).matrix();
        }
    };
}