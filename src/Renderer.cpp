#include"Renderer.hpp"
#include<iostream>

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR)
        ;
}

bool GLLogCall(const char *function, const char *file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error:] (" << error << ") "
                  << function << " "
                  << file << ":" << "[line no]: " << line << std::endl;
        return false;
    }
    return true;
}

void Renderer::Draw(const VertexArray &va, IndexBuffer &ib, const Shader &shader) const
{
    shader.Bind();
    va.Bind();
    ib.Bind();
    glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr);

}

void Renderer::Clear() const
{
    glClear(GL_COLOR_BUFFER_BIT);
}

