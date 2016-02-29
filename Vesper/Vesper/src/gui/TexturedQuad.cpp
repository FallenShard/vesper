#include <vector>

#include <gui/TexturedQuad.hpp>

namespace vesp
{
    TexturedQuad::TexturedQuad()
    {
        std::vector<GLfloat> m_vertices;
        std::vector<GLushort> m_indices;

        m_vertices.push_back(-1.f);
        m_vertices.push_back(-1.f);
        m_vertices.push_back(0.f);
        m_vertices.push_back(0.f);

        m_vertices.push_back(1.f);
        m_vertices.push_back(-1.f);
        m_vertices.push_back(1.f);
        m_vertices.push_back(0.f);

        m_vertices.push_back(1.f);
        m_vertices.push_back(1.f);
        m_vertices.push_back(1.f);
        m_vertices.push_back(1.f);

        m_vertices.push_back(-1.f);
        m_vertices.push_back(1.f);
        m_vertices.push_back(0.f);
        m_vertices.push_back(1.f);

        m_indices.push_back(0);
        m_indices.push_back(1);
        m_indices.push_back(2);

        m_indices.push_back(0);
        m_indices.push_back(2);
        m_indices.push_back(3);

        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        glGenBuffers(1, &m_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(GLfloat), m_vertices.data(), GL_STATIC_DRAW);

        glGenBuffers(1, &m_ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLushort), m_indices.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (const void*)0);

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (const void*)(2 * sizeof(GLfloat)));

        glBindVertexArray(0);

        glGenTextures(1, &m_texId);
        glBindTexture(GL_TEXTURE_2D, m_texId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    TexturedQuad::~TexturedQuad()
    {
        glDeleteVertexArrays(1, &m_vao);

        glDeleteBuffers(1, &m_vbo);
        glDeleteBuffers(1, &m_ibo);

        glDeleteTextures(1, &m_texId);
    }

    void TexturedQuad::setTechnique(GLuint technique)
    {
        m_technique = technique;

        GLuint texUnif = glGetUniformLocation(m_technique, "tex");

        glUseProgram(m_technique);
        glUniform1i(texUnif, 1);
    }

    void TexturedQuad::setTextureSize(int width, int height)
    {
        std::vector<GLfloat> emptyData(width * height * 3, 0);
        glActiveTexture(GL_TEXTURE0 + 1);
        glBindTexture(GL_TEXTURE_2D, m_texId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, emptyData.data());
    }

    void TexturedQuad::setTexture(const float* data, int width, int height)
    {
        glActiveTexture(GL_TEXTURE0 + 1);
        glBindTexture(GL_TEXTURE_2D, m_texId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, data);
    }

    void TexturedQuad::updateTexture(const float* data, int xOffset, int yOffset, int width, int height)
    {
        glActiveTexture(GL_TEXTURE0 + 1);
        glBindTexture(GL_TEXTURE_2D, m_texId);
        glTexSubImage2D(GL_TEXTURE_2D, 0, xOffset, yOffset, width, height, GL_RGB, GL_FLOAT, data);
    }

    void TexturedQuad::render()
    {
        glUseProgram(m_technique);

        glBindVertexArray(m_vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
    }
}