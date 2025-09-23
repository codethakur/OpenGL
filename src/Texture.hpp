#pragma once
#include "Renderer.hpp"

class Texture
{
private:
    unsigned int m_rendererId;
    std::string m_filePath;
    unsigned char *m_LocalBuffer;
    int m_Width, m_Height, m_BPP;

public:
    Texture(const std::string &path);
    ~Texture();

    void Bind(unsigned int sloat = 0) const;
    void UnBind()const;

    inline int getWidth() const { return m_Width; }
    inline int getHeight() const { return m_Height; }
};
