#pragma once
#include <string>
#include <unordered_map>
#include <glad/glad.h>

class Shader
{
private:
    unsigned int m_renderedId;
    std::string m_filePath;
    mutable std::unordered_map<std::string, int> m_uniformLoactionCache;

public:
    Shader(const std::string &filepath);
    ~Shader();

    void Bind()const;
    void UnBind();
    // SetUniform4f → color
    void setUniform1i(const std::string &name, int value);
    // SetUniform1f → opacity
    void setUniform1f(const std::string &name, float value);

    // current used→
    void setUniform4f(const std::string &name, float v0, float v1, float v2, float v3);

private:
    struct shaderProgrammingSources
    {
        std::string vertexSource;
        std::string fragmentSource;
    };

    shaderProgrammingSources parseShader(const std::string &filepath);
    unsigned int compileShader(unsigned int type, const std::string &source);
    unsigned int createShader(const std::string &vertexShader, const std::string &fragmentshader);
    int getUniformLocation(const std::string &name);
};
