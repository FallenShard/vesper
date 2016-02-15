#pragma once

#include <vector>

#include <acceleration/EmbreeWrapper.hpp>

#include <cameras/Camera.hpp>
#include <sampling/Sampler.hpp>
#include <shapes/Shape.hpp>

namespace vesp
{
    class Scene
    {
    public:
        Scene(int width, int height);
        ~Scene();

        const Camera* getCamera() const;
        const Sampler* getSampler() const;

        bool rayIntersect(const Ray3f& ray, Intersection& its) const;
        
    private:
        EmbreeWrapper m_embreeWrapper;

        std::vector<Shape*> m_shapes;

        Camera* m_camera;
    };
}