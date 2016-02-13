#pragma once

#include <string>
#include <map>
#include <memory>

#include <glad/glad.h>

#include <glfw/glfw3.h>

#include "core/TexturedQuad.hpp"
#include "core/Bitmap.hpp"

class Renderer
{
public:
    Renderer(GLFWwindow* mainWindow);
    ~Renderer();

    void render();

    GLuint prepareShader(GLenum type, const std::string& fileName);
    GLuint prepareTechnique(const std::string& name, GLuint vs, GLuint fs);
    GLuint getTechnique(const std::string& name);

    void onResized(int newWidth, int newHeight);
    void onKeyPressed(int key, int action, int mode);

private:
    GLFWwindow* m_window;

    std::map<std::string, GLuint> m_techniques;

    std::shared_ptr<TexturedQuad> m_texQuad;

    std::shared_ptr<Bitmap> m_bitmap;
};