#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
struct shaderProgramsource
{
    std::string VertexSource;
    std::string FragmentSource;
};

static shaderProgramsource parseShader(const std::string &filepath)
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

static unsigned int Compiledshader(unsigned int type, const std::string &source)
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
        std::cerr << "[Failed:] to compile "
                  << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
                  << " shader\n";
        std::cerr << message << std::endl;

        glDeleteShader(id);
        return 0;
    }

    return id;
}

static unsigned int Createshader(const std::string &vertexShader, const std::string &fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = Compiledshader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = Compiledshader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int main(void)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(640, 480, "OpenGL Window", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    // VAO is required in core profile (macOS)
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Vertex position data (x, y)
    float positions[] = {
        -0.5f,
        -0.5f,
        0.0f,
        0.5f,
        0.5f,
        -0.5f,
    };

    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

    // Send 2 floats per vertex, OpenGL fills z=0.0, w=1.0 → vec4
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    // unsigned int shader = Createshader(vertexShader, fragmentShader);
    // glUseProgram(shader);

    shaderProgramsource source = parseShader("res/shaders/Basic.shader");
    std::cout << "\n---------Vertex---------" << std::endl;
    std::cout << source.VertexSource << std::endl;
    std::cout << "---------Fragment---------" << std::endl;
    std::cout << source.FragmentSource << std::endl;

    unsigned int shader = Createshader(source.VertexSource, source.FragmentSource);

    glUseProgram(shader);
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
