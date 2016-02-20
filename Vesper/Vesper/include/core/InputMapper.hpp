#pragma once

#include <set>
#include <memory>

#include <GLFW/glfw3.h>

#include <core/EventSource.hpp>

namespace vesp
{
    class InputMapper
    {
    public:
        InputMapper(GLFWwindow* window);
        ~InputMapper();

        static void resizeEventHandlerCallback(GLFWwindow* window, int width, int height);
        EventSource<void, int, int> windowResized;

        static void keyEventHandlerCallback(GLFWwindow* window, int key, int scanCode, int action, int mode);
        EventSource<void, int, int, int> keyPressed;

        static void mouseMoveEventHandlerCallback(GLFWwindow* window, double xPos, double yPos);
        EventSource<void, double, double> mouseMoved;

        static void mouseButtonClickEventHandlerCallback(GLFWwindow* window, int button, int action, int mods);
        EventSource<void, int, int, int, double, double> mouseClicked;

    private:
        GLFWwindow* m_window;
    };
}