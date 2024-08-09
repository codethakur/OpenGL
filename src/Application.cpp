#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
x;\
ASSERT(GLLogCall(#x, __FILE__, __LINE__))

static void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error](" << error << "): " << function << " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}

int main(void)
{
    // Initialize GLFW
    if (!glfwInit())
        return -1;

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(640, 480, "OpenGL", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glfwSwapInterval(1);
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Error initializing GLEW!" << std::endl;
        return -1;
    }

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    // Define vertex positions and indices
    float positions[] = {
        -0.5f, -0.5f, // Bottom-left
         0.5f, -0.5f, // Bottom-right
         0.5f,  0.5f, // Top-right
        -0.5f,  0.5f  // Top-left
    };
    unsigned int indices[] = {
        0, 1, 2, // First triangle
        2, 3, 0  // Second triangle
    };

    // Set up VAO, VBO, and IBO
    VertexArray va;
    VertexBuffer vb(positions, sizeof(positions));
    VertexBufferLayout layout;
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);

    IndexBuffer ib(indices, sizeof(indices) / sizeof(unsigned int));

    // Load and compile shaders
    ShaderProgramSource source = Shader::ParseShader("res/shaders/Basic.shader");
    unsigned int shader = Shader::CreateShader(source.VertexSource, source.FragmentSource);

    if (shader == 0) {
        std::cerr << "Failed to create shader program!" << std::endl;
        return -1;
    }

    glUseProgram(shader);

    // Get uniform location and set initial color
    int location = glGetUniformLocation(shader, "u_color");
    ASSERT(location != -1);

    float r = 0.0f;
    float increment = 0.05f;

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        va.Bind();
        ib.Bind();

        // Update uniform color
        glUniform4f(location, r, 0.5f, 0.5f, 1.0f);

        // Draw elements
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

        // Update color for animation
        r += increment;
        if (r > 1.0f)
            increment = -0.05f;
        else if (r < 0.0f)
            increment = 0.05f;

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up
    glDisableVertexAttribArray(0);
    glfwTerminate();
    return 0;
}
