#define IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_IMPL_OPENGL_LOADER_GLAD   

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
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw.h"
#include "vendor/imgui/imgui_impl_opengl3.h"

static void glfw_error_callback(int error, const char* description)
{
    std::cerr << "GLFW Error (" << error << "): " << description << '\n';
}


int main(void)
{
    // ----------------- Init -----------------
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
    {
        std::cerr << "❗️Failed to init GLFW\n";
        return -1;
    }

    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(940, 680, "OpenGL Window 🖥️", NULL, NULL);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow*, int width, int height) {
        glViewport(0, 0, width, height);
    });

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
        -0.5f, -0.5f, 0.0f, 0.0f, // 0
        0.5f, -0.5f, 1.0f, 0.0f,  // 1
        0.5f, 0.5f, 1.0f, 1.0f,   // 2
        -0.5f, 0.5f, 0.0f, 1.0f   // 3
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0};
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    VertexArray va;
    VertexBuffer vb(positions, 4 * 4 * sizeof(float));

    VertexBufferLayout layout;
    layout.Push<float>(2); // position -> 2 floats
    layout.Push<float>(2); // texCoord -> 2 floats
    va.addBuffer(vb, layout);

    IndexBuffer ib(indices, 6);

    // ----------------- Matrix -----------------
   int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    float aspect = (float)width / (float)height;

    // Keep 6 units across X (same visual width)
    float orthoHeight = 6.0f / aspect;

    glm::mat4 proj = glm::ortho(-3.0f, 3.0f, -orthoHeight / 2.0f, orthoHeight / 2.0f, -1.0f, 1.0f);


    // // ----------------- Shader -----------------
    Shader shader("res/shaders/Basic.shader");
    shader.Bind();

    // ----------------- Texture -----------------
    Texture texture("res/textures/codethakur.png");
    texture.Bind(0);                     // bind to slot 0
    shader.setUniform1i("u_Texture", 0); // tell shader to use slot 0
    shader.setUniformMat4f("u_MVP", proj);

    // ----------------- Renderer -----------------
    Renderer renderer;
    float r = 0.0f;
    float increment = 0.05f;
    float moveXA = -1.5f, moveYA = 0.0f, speedA = 0.0f;
    float moveXB = 0.0f, moveYB = 0.0f, speedB = 0.0f;
    float angleA = 0.0f, angleB = 0.0f;
    float objectBrightness = 1.0f;     // affects u_Color intensity
    float backgroundBrightness = 1.0f; // affects glClearColor intensity

    // ----------------- ImGui Setup -----------------
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
    

    // ----------------- App State -----------------
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.71f, 0.74f, 0.76f, 1.00f);


    // ----------------- Loop -----------------

    
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // --- ImGui Frame ---
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // --- ImGui Controls ---
        {
            ImGui::Begin("Hello, from ImGui");
            ImGui::Checkbox("Demo Window", &show_demo_window);
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::Text("Object A Controls");
            ImGui::SliderFloat("Move X A", &moveXA, -2.0f, 2.0f);
            ImGui::SliderFloat("Move Y A", &moveYA, -1.5f, 1.5f);
            ImGui::SliderFloat("Rotate Speed A", &speedA, -1.0f, 1.0f);

            ImGui::Separator();
            ImGui::Text("Object B Controls");
            ImGui::SliderFloat("Move X B", &moveXB, -2.0f, 2.0f);
            ImGui::SliderFloat("Move Y B", &moveYB, -1.5f, 1.5f);
            ImGui::SliderFloat("Rotate Speed B", &speedB, -1.0f, 1.0f);

            ImGui::ColorEdit3("clear color", (float*)&clear_color);
            ImGui::Separator();

           

            if (ImGui::Button("Reset All"))
            {
                moveXA = -1.5f ;
                moveYA = moveXB = moveYB = 0.0f;
                speedA = speedB = 0.0f;
                angleA = angleB = 0.0f;
                objectBrightness = 1.0f;
                backgroundBrightness = 1.0f;
               clear_color = ImVec4(0.71f, 0.74f, 0.76f, 1.00f);
            }

            ImGui::End();

            if (show_another_window)
            {
                ImGui::Begin("Another Window", &show_another_window);
                ImGui::Text("Hello from another window!");
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                            1000.0f / io.Framerate, io.Framerate);
                ImGui::Text("Lighting Controls" );
                ImGui::SliderFloat("Object Brightness", &objectBrightness, 0.0f, 2.0f, "%.2f");
                ImGui::SliderFloat("Background Brightness", &backgroundBrightness, 0.0f, 2.0f, "%.2f");            
                ImGui::End();
            }
        }

        // --- Logic Update ---
        angleA += speedA;
        angleB += speedB;

          // --- Clamp positions (equal edge distance on X and Y) ---
        const float halfW = 0.5f; // object half width
        const float halfH = 0.5f; // object half height
        const float left   = -3.0f + halfW;
        const float right  =  3.0f - halfW;
        const float bottom = -orthoHeight / 2.0f + halfH;
        const float top    =  orthoHeight / 2.0f - halfH;

        moveXA = std::clamp(moveXA, left, right);
        moveXB = std::clamp(moveXB, left, right);
        moveYA = std::clamp(moveYA, bottom, top);
        moveYB = std::clamp(moveYB, bottom, top);


        // --- Clear and Render Scene ---
        ImVec4 adjustedClearColor = clear_color * backgroundBrightness;
        glClearColor(adjustedClearColor.x, adjustedClearColor.y, adjustedClearColor.z, adjustedClearColor.w);
        renderer.Clear();

       
        shader.Bind();
        texture.Bind(0);
        shader.setUniform4f("u_Color",
                    1.0f * objectBrightness,
                    r * objectBrightness,
                    0.2f * objectBrightness,
                    1.0f);


        glm::mat4 view = glm::mat4(1.0f); // static camera

        // Object A
        glm::mat4 modelA = glm::mat4(1.0f);
        modelA = glm::translate(modelA, glm::vec3(moveXA, moveYA, 0.0f));
        modelA = glm::rotate(modelA, angleA, glm::vec3(0.0f, 0.0f, 1.0f));

        // Object B
        glm::mat4 modelB = glm::mat4(1.0f);
        modelB = glm::translate(modelB, glm::vec3(moveXB, moveYB, 0.0f));
        modelB = glm::rotate(modelB, angleB, glm::vec3(0.0f, 0.0f, 1.0f));

        // Render both
        std::vector<glm::mat4> models = { modelA, modelB };
        for (const auto& model : models)
        {
            glm::mat4 mvp = proj * view * model;
            shader.setUniformMat4f("u_MVP", mvp);
            renderer.Draw(va, ib, shader);
        }

        // --- Animate color oscillation ---
        if (r > 1.0f) increment = -0.05f;
        else if (r < 0.0f) increment = 0.05f;
        r += increment;

        // --- ImGui Render ---
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // ----------------- Cleanup -----------------
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
