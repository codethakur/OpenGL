#include "BackgroundRenderer.hpp"
#include "VertexBufferLayout.hpp"
#include "Renderer.hpp"

BackgroundRenderer::BackgroundRenderer()
    : m_Shader("res/shaders/Background.shader")
{
    float positions[] = {
    -1.0f, -1.0f,  // bottom-left
     1.0f, -1.0f,  // bottom-right
    -1.0f,  1.0f,  // top-left
     1.0f,  1.0f   // top-right
};


    

    m_VB = new VertexBuffer(positions, sizeof(positions));
    VertexBufferLayout layout;
    layout.Push<float>(2);
    m_VA.addBuffer(*m_VB, layout);
    
}

BackgroundRenderer::~BackgroundRenderer()
{
    delete m_VB;
}

void BackgroundRenderer::Draw() const
{
    m_Shader.Bind();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
