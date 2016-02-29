#pragma once

#include <glad/glad.h>

namespace vesp
{
    class TexturedQuad
    {
    public:
        TexturedQuad();
        ~TexturedQuad();

        void setTechnique(GLuint technique);
        void setTextureSize(int width, int height);
        void setTexture(const float* data, int width, int height);
        void updateTexture(const float* data, int xOffset, int yOffset, int width, int height);
        void render();

    private:
        GLuint m_vao;
        GLuint m_vbo;
        GLuint m_ibo;
        GLuint m_texId;

        GLuint m_technique;
    };
}