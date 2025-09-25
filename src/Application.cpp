#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include "Renderer.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "VertexArray.hpp"
#include "VertexBufferLayout.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include"vendor/glm/gtc/matrix_transform.hpp"

int main(void)
{
    // ----------------- Init -----------------
    if (!glfwInit())
    {
        std::cerr << "❗️Failed to init GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(640, 480, "OpenGL Window 🖥️", NULL, NULL);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // V-Sync

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "❗️Failed to init GLAD\n";
        return -1;
    }

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    // ----------------- Vertex Data -----------------
    float positions[] = {
        //   X    Y      U       V
        -0.5f, -0.5f,   0.0f,   0.0f, // 0
         0.5f, -0.5f,   1.0f,   0.0f, // 1
         0.5f,  0.5f,   1.0f,   1.0f, // 2
        -0.5f,  0.5f,   0.0f,   1.0f  // 3
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0};
    // GLCall(glEnable(GL_BLEND));
    // GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    VertexArray va;
    VertexBuffer vb(positions, 4 * 4 * sizeof(float));

    VertexBufferLayout layout;
    layout.Push<float>(2); // position -> 2 floats
    layout.Push<float>(2); // texCoord -> 2 floats
    va.addBuffer(vb, layout);

    IndexBuffer ib(indices, 6);

    // ----------------- Matrix -----------------
    glm::mat4 proj = glm::ortho(-3.0f, 3.0f, -2.0f, 2.0f, -1.0f, 1.0f);

    //glm::mat4 proj  = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);
    glm::mat4 view  = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, .0f));//camera view
    glm::mat4 model = glm::mat4(1.0f);    //rotate a cube, move it left/right, scale it bigger/smaller.
    glm::mat4 mvp = proj * view * model; // proj * view * model=> order
    // ----------------- Shader -----------------
    Shader shader("res/shaders/Basic.shader");
    shader.setUniformMat4f("u_MVP", proj);
    shader.Bind();
    // int loc = getUniformLocation("u_MVP");
    // std::cout << "u_MVP location = " << loc << std::endl;

    // ----------------- Texture -----------------
    Texture texture("res/textures/codethakur.png");
    texture.Bind(0);                     // bind to slot 0
    shader.setUniform1i("u_Texture", 0); // tell shader to use slot 0
    shader.setUniformMat4f("u_MVP",  proj);

    // ----------------- Renderer -----------------
    Renderer renderer;
    float r = 0.0f;
    float increment = 0.05f;

    // ----------------- Loop -----------------
    float angle = 0.0f;
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.235f, 0.235f, 0.235f, 0.05f);
        renderer.Clear();

        shader.Bind();
        texture.Bind(0);

        // animate color tint
        shader.setUniform4f("u_Color", 1.0f, r, 0.2f, 1.0f);
        // ----------------- View (camera, dynamic)Basically-> Move=>Left/Right + Top/Bottom -----------------
        float t = (float)glfwGetTime();
        glm::mat4 view = glm::translate(glm::mat4(1.0f),
                                        glm::vec3(-sin(t) , cos(t), 0.0f));

        // ----------------- Model (object, dynamic) -----------------
        angle += 0.02f;
        glm::mat4 model = glm::rotate(glm::mat4(1.0f), angle,  glm::vec3(0.5f, 0.5f, 1.0f));

        // ----------------- Final MVP -----------------
        glm::mat4 mvp = proj * view * model;
        shader.setUniformMat4f("u_MVP", mvp);
    
        renderer.Draw(va, ib, shader);


        // animate "r"
        if (r > 1.0f)
            increment = -0.05f;

        else if (r < 0.0f)
            increment = 0.05f;
        r += increment;
       // angle += increment;

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}


