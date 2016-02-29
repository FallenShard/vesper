#include <iostream>
#include <core/Application.hpp>

namespace
{
    const unsigned int DefaultWidth = 800;
    const unsigned int DefaultHeight = 600;
}

namespace vesp
{
    Application::Application()
    {
    }

    Application::~Application()
    {
        glfwTerminate();
    }

    Application::Status Application::initializeDependencies()
    {
        // Initialize GLFW window management library
        if (!glfwInit())
        {
            std::cout << "Failed to initialize GLFW windowing library." << std::endl;
            return Status::Failure;
        }

        return Status::OK;
    }

    Application::Status Application::initializeComponents()
    {
        // Set some window attributes to be used for next window creation
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

        // Create the main window
        m_mainWindow = glfwCreateWindow(DefaultWidth, DefaultHeight, "Vesper", nullptr, nullptr);
        if (!m_mainWindow)
        {
            glfwTerminate();
            return Status::Failure;
        }

        // This call here is needed for glad GL loader
        glfwMakeContextCurrent(m_mainWindow);

        // Load up OpenGL extensions
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize OpenGL context with extensions." << std::endl;
            return Status::Failure;
        }

        const GLubyte* glVersion = glGetString(GL_VERSION);
        const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

        std::cout << "OpenGL Version: " << glVersion[0] << "." << glVersion[2] << std::endl;
        std::cout << "GLSL Version: " << glslVersion[0] << "." << glslVersion[2] << std::endl;

        // InputMapper is used to respond to basic window event callbacks
        m_inputMapper = std::make_shared<InputMapper>(m_mainWindow);

        // Renderer is responsible for getting stuff drawn onto the screen
        m_renderer = std::make_shared<Renderer>(m_mainWindow);

        // Ray tracer that loads up the scene and performs the ray tracing task 
        m_rayTracer = std::make_unique<RayTracer>();

        // Connect a few events and handlers
        m_inputMapper->keyPressed.subscribe<Application, &Application::keyEventDebug>(this);
        m_inputMapper->filesDropped.subscribe<Application, &Application::onFilesDropped>(this);
        m_inputMapper->keyPressed.subscribe<Renderer, &Renderer::onKeyPressed>(m_renderer.get());

        m_rayTracer->sceneInitialized.subscribe<Application, &Application::onSceneInitialized>(this);
        m_rayTracer->imageUpdated.subscribe<Renderer, &Renderer::onImageUpdated>(m_renderer.get());

        // Genetic algorithm for solving TSP problem, fun stuff
        //m_tsp = std::make_shared<TravellingSalesman>(50, 75);
        //m_tsp->setShaderProgram(m_renderer->getTechnique("color"));
        //m_tsp->generatePopulation();

        return Status::OK;
    }

    void Application::run()
    {
        glClearColor(0.0f, 0.3f, 0.0f, 1.0f);

        while (!glfwWindowShouldClose(m_mainWindow))
        {
            // Wait for events, saves processing power
            glfwWaitEvents();

            // Render the content to the back buffer
            m_renderer->render();

            // Swap the back and front buffers
            glfwSwapBuffers(m_mainWindow);
        }
    }

    void Application::keyEventDebug(int key, int action, int mode)
    {
        std::cout << "Key " << key << " action: " << action << " mode: " << mode << std::endl;

        if (key == GLFW_KEY_S)
        {
            m_rayTracer->stopRayTracing();
        }
        else if (key == GLFW_KEY_E)
        {
            m_rayTracer->initializeScene("res/example.xml");
            m_rayTracer->startRayTracing();
        }
    }

    void Application::onSceneInitialized(int width, int height)
    {
        glfwSetWindowSize(m_mainWindow, width, height);
        m_renderer->setImageSize(width, height);
    }

    void Application::onFilesDropped(std::vector<std::string> filenames)
    {
        m_rayTracer->initializeScene(filenames[0]);
        m_rayTracer->startRayTracing();
    }
}