#pragma once

#include <string>
#include <map>
#include <memory>

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <tbb/concurrent_queue.h>

#include <core/Bitmap.hpp>
#include <core/ImageBlock.hpp>

namespace vesp
{
    class TexturedQuad;

    class Renderer
    {
    public:
        Renderer(GLFWwindow* mainWindow);
        ~Renderer();

        void render();

        GLuint prepareShader(GLenum type, const std::string& fileName);
        GLuint prepareTechnique(const std::string& name, GLuint vs, GLuint fs);
        GLuint getTechnique(const std::string& name);

        void resize(int newWidth, int newHeight);
        void setImageSize(int newWidth, int newHeight);

        void onKeyPressed(int key, int action, int mode);
        void onImageUpdated(ImageBlock& imageBlock, int xOffset, int yOffset);

    private:
        GLFWwindow* m_window;

        std::map<std::string, GLuint> m_techniques;

        std::shared_ptr<TexturedQuad> m_texQuad;

        std::shared_ptr<Bitmap> m_bitmap;

        struct RayTracingUpdate
        {
            std::vector<float> data;
            int w, h, x, y;
        };

        tbb::concurrent_queue<RayTracingUpdate> m_updateQueue;
    };
}