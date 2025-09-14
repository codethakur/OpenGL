#pragma once

#include <vector>
#include <glad/glad.h>
#include <cassert>

struct VertexBufferElement
{
    unsigned int type;
    unsigned int count;
    unsigned char normalized;

    static unsigned int getSizeOfType(unsigned int type);
};

class VertexBufferLayout
{
private:
    std::vector<VertexBufferElement> m_Elements;
    unsigned int m_Stride;

public:
    VertexBufferLayout();

    template<typename T>
    void Push(unsigned int count)
    {
        static_assert(sizeof(T) == 0, "Unsupported type in VertexBufferLayout!");
    }

    template<>
    void Push<float>(unsigned int count);

    template<>
    void Push<unsigned int>(unsigned int count);

    template<>
    void Push<unsigned char>(unsigned int count);

    inline const std::vector<VertexBufferElement>& getElements() const { return m_Elements; }
    inline unsigned int getStride() const { return m_Stride; }
};
