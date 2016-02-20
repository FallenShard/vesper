#pragma once

#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <core/InputMapper.hpp>
#include <core/RayTracer.hpp>
#include <core/ImageBlock.hpp>

#include <gui/Renderer.hpp>

#include <genetic/TravellingSalesman.hpp>

namespace vesp
{
    class Application
    {
    public:
        enum class Status
        {
            OK,
            Failure
        };

        Application();
        ~Application();

        Status initializeDependencies();
        Status initializeComponents();

        void run();

        void keyEventDebug(int key, int action, int mode);
        void onSceneInitialized(int width, int height);

    private:
        GLFWwindow* m_mainWindow;

        std::shared_ptr<InputMapper> m_inputMapper;

        std::shared_ptr<Renderer> m_renderer;

        std::shared_ptr<TravellingSalesman> m_tsp;

        std::unique_ptr<RayTracer> m_rayTracer;
    };
}