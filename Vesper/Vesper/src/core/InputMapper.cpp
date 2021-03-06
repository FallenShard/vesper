#include <core/InputMapper.hpp>

namespace vesp
{
    InputMapper::InputMapper(GLFWwindow* window)
        : m_window(window)
    {
        glfwSetWindowUserPointer(m_window, this);

        glfwSetKeyCallback(m_window, InputMapper::keyEventHandlerCallback);
        glfwSetWindowSizeCallback(m_window, InputMapper::resizeEventHandlerCallback);
        glfwSetCursorPosCallback(m_window, InputMapper::mouseMoveEventHandlerCallback);
        glfwSetMouseButtonCallback(m_window, InputMapper::mouseButtonClickEventHandlerCallback);
        glfwSetDropCallback(m_window, InputMapper::fileDropEventHandlerCallback);
    }

    InputMapper::~InputMapper()
    {
    }

    void InputMapper::keyEventHandlerCallback(GLFWwindow* window, int key, int scanCode, int action, int mode)
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);

        auto inputMapper = reinterpret_cast<InputMapper*>(glfwGetWindowUserPointer(window));
        if (inputMapper) inputMapper->keyPressed(key, action, mode);
    }

    void InputMapper::resizeEventHandlerCallback(GLFWwindow* window, int width, int height)
    {
        auto inputMapper = reinterpret_cast<InputMapper*>(glfwGetWindowUserPointer(window));
        if (inputMapper) inputMapper->windowResized(width, height);
    }

    void InputMapper::mouseMoveEventHandlerCallback(GLFWwindow* window, double xPos, double yPos)
    {
        auto inputMapper = reinterpret_cast<InputMapper*>(glfwGetWindowUserPointer(window));
        if (inputMapper) inputMapper->mouseMoved(xPos, yPos);
    }

    void InputMapper::mouseButtonClickEventHandlerCallback(GLFWwindow* window, int button, int action, int mods)
    {
        auto inputMapper = reinterpret_cast<InputMapper*>(glfwGetWindowUserPointer(window));
        double xPos, yPos;
        glfwGetCursorPos(window, &xPos, &yPos);
        if (inputMapper) inputMapper->mouseClicked(button, action, mods, xPos, yPos);
    }

    void InputMapper::fileDropEventHandlerCallback(GLFWwindow* window, int numFiles, const char** fileNames)
    {
        auto inputMapper = reinterpret_cast<InputMapper*>(glfwGetWindowUserPointer(window));
        std::vector<std::string> filenames;
        for (int i = 0; i < numFiles; i++)
        {
            filenames.push_back(fileNames[i]);
        }
        if (inputMapper) inputMapper->filesDropped(filenames);
    }
}