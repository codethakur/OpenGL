#include"Renderer.hpp"
#include "Console.hpp"
#include<iostream>

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR)
        ;
}

bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        Console::LOGN("[OpenGL ERROR] (" + std::to_string(error) + ")", Color::RED);
        Console::LOGN("Function: " + std::string(function), Color::YELLOW);
        Console::LOGN("Location: " + std::string(file) + ":" + std::to_string(line), Color::YELLOW);
        return false;
    }
    return true;
}


void Renderer::Draw(const VertexArray &va, IndexBuffer &ib, const Shader &shader) const
{
    shader.Bind();
    va.Bind();
    ib.Bind();
    GLCall(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr));

}

void Renderer::Clear() const
{
    glClear(GL_COLOR_BUFFER_BIT);
}

