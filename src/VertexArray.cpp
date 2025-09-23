#include "VertexArray.hpp"
#include <glad/glad.h>
#include "Renderer.hpp"
VertexArray::VertexArray()
{
   GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
   GLCall( glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::Bind() const
{
   GLCall( glBindVertexArray(m_RendererID));
}

void VertexArray::UnBind() const
{
   GLCall( glBindVertexArray(0));
}

void VertexArray::addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
 GLCall (  Bind());
   GLCall( vb.Bind());

    const auto& elements = layout.getElements();
    unsigned int offset = 0;
    unsigned int index = 0; 
    for (const auto&element:elements)
    {

       GLCall( glEnableVertexAttribArray(index));
        GLCall( glVertexAttribPointer(
            index,
            element.count,
            element.type,
            element.normalized,
            layout.getStride(),
            (const void*)(uintptr_t)offset
        );
        offset += element.count * VertexBufferElement::getSizeOfType(element.type));
         index++;
    }
}
