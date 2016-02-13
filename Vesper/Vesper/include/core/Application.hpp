#pragma once

#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "core/InputMapper.hpp"
#include "core/Renderer.hpp"
#include "core/Bitmap.hpp"
#include "core/TravellingSalesman.hpp"

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

private:
    GLFWwindow* m_mainWindow;

    std::shared_ptr<InputMapper> m_inputMapper;

    std::shared_ptr<Renderer> m_renderer;
    std::shared_ptr<Bitmap> m_bitmap;

    std::shared_ptr<TravellingSalesman> m_tsp;
};