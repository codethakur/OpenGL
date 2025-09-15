#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Renderer.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "VertexArray.hpp"
#include "VertexBufferLayout.hpp"
#include "Shader.hpp"



int main(void)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_FALSE);

    GLFWwindow *window = glfwCreateWindow(640, 480, "OpenGL Window🖥️", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    // Vertex position data (x, y)
    float positions[] = {
        -0.5f,   -0.5f,  // 0
         0.5f,   -0.5f,  // 1
         0.5f,    0.5f,  // 2
        -0.5f,    0.5f   // 3
    };

    unsigned int indices[] =
        {
            0, 1, 2,
            2, 3, 0};
    VertexArray va;
    VertexBuffer vb(positions, 4 * 2 * sizeof(float));

    VertexBufferLayout layout;
    layout.Push<float>(2); // each vertex has 2 floats (x,y)

    va.addBuffer(vb, layout);

    IndexBuffer ib(indices, 6);
    Shader shader("res/shaders/Basic.shader");
    shader.Bind();
    shader.setUniform4f("u_Color", 0.8f, 0.3f, 0.2f, 1.0f);

    va.UnBind();
    ib.UnBind();
    vb.UnBind();
    shader.UnBind();

    float r = 0.0f;
    float increment = 0.05f;

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.Bind();
        shader.setUniform4f("u_Color", 1.0, r, 0.2, 1.0);

        va.Bind();  
        ib.Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        // animate green component
        if (r > 1.0f)
            increment = -0.05f;
        else if (r < 0.0f)
            increment = 0.05f;
        r += increment;

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
