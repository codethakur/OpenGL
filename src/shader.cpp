#include "Shader.hpp"
#include "Renderer.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(const std::string &filepath) : m_renderedId(0), m_filePath(filepath)
{
    shaderProgrammingSources source = parseShader(filepath);
    m_renderedId = createShader(source.vertexSource, source.fragmentSource);
}
Shader::~Shader()
{
    GLCall(glDeleteProgram(m_renderedId));
}

Shader::shaderProgrammingSources Shader::parseShader(const std::string &filepath)
{
    std::ifstream stream(filepath);

    enum class shaderTypes
    {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    shaderTypes type = shaderTypes::NONE;

    while (std::getline(stream, line))
    {
        if (line.find("shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                type = shaderTypes::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = shaderTypes::FRAGMENT;
            }
        }
        else
        {
            ss[(int)type] << line << "\n";
        }
    }
    return {ss[0].str(), ss[1].str()};
}

unsigned int Shader::compileShader(unsigned int type, const std::string &source)
{
    unsigned int id = glCreateShader(type);
    const char *src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char *message = (char *)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cerr << "[❗️Failed:] to compile "
                  << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
                  << " shader\n";
        std::cout << message << std::endl;

        glDeleteShader(id);
        return 0;
    }

    return id;
}

unsigned int Shader::createShader(const std::string &vertexShader, const std::string &fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}
void Shader::Bind()const
{
    GLCall(glUseProgram(m_renderedId));
}
void Shader::UnBind()
{
    GLCall(glUseProgram(0));
}

int Shader::getUniformLocation(const std::string &name)
{

    if (m_uniformLoactionCache.find(name) != m_uniformLoactionCache.end())
    {
        return m_uniformLoactionCache[name];
    }
    GLCall(int location = glGetUniformLocation(m_renderedId, name.c_str()));

    if (location == -1)
        std::clog << "[⚠️ Warning: ] uniform '" << name << "' doesn't exits\n";
    m_uniformLoactionCache[name] = location;
    return location;
}

void Shader::setUniform1i(const std::string &name, int value)
{
    GLCall(glUniform1i(getUniformLocation(name), value));
}
void Shader::setUniform1f(const std::string &name, float value)
{
    GLCall(glUniform1f(getUniformLocation(name), value));
}
void Shader::setUniform4f(const std::string &name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(getUniformLocation(name), v0, v1, v2, v3));
}

void Shader::setUniformMat4f(const std::string &name, const glm::mat4& matrix)
{
    GLint loc = getUniformLocation(name);
    if (loc == -1)
    {
        std::cerr << "❌ setUniformMat4f: uniform '" << name << "' not found\n";
        return; 
    }

    glUniformMatrix4fv(loc, 1, GL_FALSE, &matrix[0][0]);
}

