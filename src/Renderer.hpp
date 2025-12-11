#pragma once
//#include <GLFW/glfw3.h>
#include "VertexArray.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"


#define ASSERT(x) \
    if (!(x))     \
        __builtin_debugtrap();
#define GLCall(x)   \
    GLClearError(); \
    x;              \
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char *function, const char *file, int line);

class Renderer
{
public:
    virtual ~Renderer() = default;
    virtual void Draw(const VertexArray& va, IndexBuffer& ib, const Shader& shader) const;
    virtual void Clear() const;
};