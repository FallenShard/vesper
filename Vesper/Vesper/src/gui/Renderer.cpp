#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <core/Utils.hpp>
#include <core/ImageBlock.hpp>

#include <gui/Renderer.hpp>
#include <gui/TexturedQuad.hpp>

namespace vesp
{
    Renderer::Renderer(GLFWwindow* mainWindow)
        : m_window(mainWindow)
    {
        int width, height;
        glfwGetWindowSize(m_window, &width, &height);
        resize(width, height);

        GLuint vs = prepareShader(GL_VERTEX_SHADER, "res/textured_vert.glsl");
        GLuint fs = prepareShader(GL_FRAGMENT_SHADER, "res/textured_frag.glsl");
        GLuint tech = prepareTechnique("textured", vs, fs);

        vs = prepareShader(GL_VERTEX_SHADER, "res/color_vert.glsl");
        fs = prepareShader(GL_FRAGMENT_SHADER, "res/color_frag.glsl");
        tech = prepareTechnique("color", vs, fs);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glPixelStorei(GL_PACK_ALIGNMENT, 1);

        m_texQuad = std::make_shared<TexturedQuad>();
        m_texQuad->setTechnique(getTechnique("textured"));

        m_bitmap = std::make_shared<Bitmap>(1, 1);
    }

    Renderer::~Renderer()
    {
        for (auto& elem : m_techniques)
            glDeleteProgram(elem.second);
    }

    void Renderer::render()
    {
        while (!m_updateQueue.empty())
        {
            RayTracingUpdate result;
            if (m_updateQueue.try_pop(result))
            {
                m_texQuad->updateHdriTexture(result.data.data(), result.x, result.y, result.w, result.h);
            }
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_texQuad->render();
    }

    GLuint Renderer::prepareShader(GLenum type, const std::string& fileName)
    {
        GLuint shader = glCreateShader(type);

        std::string srcString = Utils::fileToString(fileName);
        const GLchar* src = srcString.c_str();
        glShaderSource(shader, 1, &src, nullptr);

        glCompileShader(shader);
        GLint isCompiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE)
        {
            std::cout << "Error compiling shader " << fileName << "!" << std::endl;
            GLint maxLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> errorLog(maxLength);
            glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);

            std::string error(errorLog.begin(), errorLog.end());
            std::cout << error << std::endl;

            glDeleteShader(shader);
            return -1;
        }

        return shader;
    }

    GLuint Renderer::prepareTechnique(const std::string& name, GLuint vs, GLuint fs)
    {
        GLuint program = glCreateProgram();

        glAttachShader(program, vs);
        glAttachShader(program, fs);

        glLinkProgram(program);
        GLint isLinked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
        if (isLinked == GL_FALSE)
        {
            std::cout << "Error linking program!" << std::endl;
            GLint maxLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> errorLog(maxLength);
            glGetProgramInfoLog(program, maxLength, &maxLength, &errorLog[0]);

            std::string error(errorLog.begin(), errorLog.end());
            std::cout << error << std::endl;

            glDeleteProgram(program);
            return -1;
        }

        glDetachShader(program, vs);
        glDetachShader(program, fs);

        glDeleteShader(vs);
        glDeleteShader(fs);

        m_techniques[name] = program;

        std::cout << "Technique \"" << name << "\" created!" << std::endl;

        return program;
    }

    GLuint Renderer::getTechnique(const std::string & name)
    {
        return m_techniques[name];
    }

    void Renderer::resize(int newWidth, int newHeight)
    {
        glViewport(0, 0, newWidth, newHeight);
    }

    void Renderer::onKeyPressed(int key, int action, int mode)
    {
    }

    void Renderer::setImageSize(int newWidth, int newHeight)
    {
        resize(newWidth, newHeight);
        m_texQuad->setTextureSize(newWidth, newHeight);
    }

    void Renderer::onImageUpdated(ImageBlock& imageBlock, int xOffset, int yOffset)
    {
        int width = imageBlock.getSize().x();
        int height = imageBlock.getSize().y();

        m_updateQueue.push(RayTracingUpdate{ imageBlock.getRaw(), width, height, xOffset, yOffset });

        // Wake up the main thread so that it goes through a render cycle
        glfwPostEmptyEvent();
    }
}