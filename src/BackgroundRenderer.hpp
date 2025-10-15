#pragma once
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "Shader.hpp"

class BackgroundRenderer
{
private:
    VertexArray m_VA;
    VertexBuffer* m_VB;
    Shader m_Shader;

public:
    BackgroundRenderer();
    ~BackgroundRenderer();

    void Draw() const;
};
