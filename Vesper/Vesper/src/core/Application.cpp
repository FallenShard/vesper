#include <iostream>



#include "core/Application.hpp"

Application::Application()
{
}

Application::~Application()
{
    glfwTerminate();
}

Application::Status Application::initializeDependencies()
{
    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return Status::Failure;
    }

    return Status::OK;
}

Application::Status Application::initializeComponents()
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    m_mainWindow = glfwCreateWindow(800, 600, "Vesper", nullptr, nullptr);
    if (!m_mainWindow)
    {
        glfwTerminate();
        return Status::Failure;
    }

    glfwMakeContextCurrent(m_mainWindow);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        return Status::Failure;
    }

    const GLubyte* glVersion = glGetString(GL_VERSION);
    const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

    std::cout << "OpenGL Version: " << glVersion[0] << "." << glVersion[2] << std::endl;
    std::cout << "GLSL Version: " << glslVersion[0] << "." << glslVersion[2] << std::endl;

    m_inputMapper = std::make_shared<InputMapper>(m_mainWindow);

    m_renderer = std::make_shared<Renderer>(m_mainWindow);

    m_tsp = std::make_shared<TravellingSalesman>(50, 75);
    m_tsp->setShaderProgram(m_renderer->getTechnique("color"));
    m_tsp->generatePopulation();

    return Status::OK;
}

void Application::run()
{
    m_inputMapper->keyPressed.subscribe<Application, &Application::keyEventDebug>(this);
    m_inputMapper->keyPressed.subscribe<Renderer, &Renderer::onKeyPressed>(m_renderer.get());
    
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    while (!glfwWindowShouldClose(m_mainWindow))
    {
        glfwWaitEvents();

        

        
        m_tsp->render();
        
        //m_renderer->render();

        glfwSwapBuffers(m_mainWindow);
    }
}

void Application::keyEventDebug(int key, int action, int mode)
{
    std::cout << "Key " << key << " action: " << action << " mode: " << mode << std::endl;

    if (key == GLFW_KEY_T && action == GLFW_RELEASE)
    {
        for (int i = 0; i < 500; i++)
        m_tsp->simulate(0);
    }
}
