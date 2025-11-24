#define IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_IMPL_OPENGL_LOADER_GLAD

#include "App.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

#include "vendor/glm/gtc/matrix_transform.hpp"
#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw.h"
#include "vendor/imgui/imgui_impl_opengl3.h"
#include "Console.hpp"

App::App()
{
    initWindow();
    initGL();
    initImGui();
    loadResources();
}

App::~App()
{
    shutdown();
}

void App::run()
{
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        update();
        render();
        renderImGui();
        glfwSwapBuffers(window);
    }
}

void App::initWindow()
{
    if (!glfwInit()) {
        Console::LOGN("Failed to init GLFW", Color::RED);
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_CORE_PROFILE, GLFW_TRUE);

    window = glfwCreateWindow(940, 680, "OpenGL  Window", NULL, NULL);
    if (!window) {
        Console::LOGN("Failed to create GLFW window", Color::RED);
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glfwSetFramebufferSizeCallback(window, [](GLFWwindow*, int w, int h){
        glViewport(0, 0, w, h);
    });
}

void App::initGL()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        Console::LOGN("Failed to init GLAD", Color::RED);
        return;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void App::initImGui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
    ImGui::StyleColorsDark();
}

void App::loadResources()
{
    gfx = new Graphicsengine(window);

    objA = gfx->createQuad("res/textures/codethakur.png");
    objB = gfx->createQuad("res/textures/codethakur.png");

   
    angleA = 0.0f;
    angleB = 0.0f;

    moveXA = -1.5f; 
    moveYA = 0.0f;
    moveXB = 0.0f;
    moveYB = 0.0f;

    speedA = 0.0f;
    speedB = 0.0f;

    r = 0.0f;
    increment = 0.05f;

    objectBrightness = 1.0f;
    backgroundBrightness = 1.0f;
}

void App::update()
{
    angleA += speedA;
    angleB += speedB;

    const float halfW = 0.5f;
    const float halfH = 0.5f;

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    float aspect = (width > 0 && height > 0) ? (float)width / (float)height : 1.0f;
    float orthoHeight = 6.0f / aspect;

    const float left = -3.0f + halfW;
    const float right = 3.0f - halfW;
    const float bottom = -orthoHeight / 2.0f + halfH;
    const float top = orthoHeight / 2.0f - halfH;

    moveXA = std::clamp(moveXA, left, right);
    moveXB = std::clamp(moveXB, left, right);
    moveYA = std::clamp(moveYA, bottom, top);
    moveYB = std::clamp(moveYB, bottom, top);

    if (r > 1.0f) increment = -0.05f;
    else if (r < 0.0f) increment = 0.05f;
    r += increment;
}

void App::render()
{
    
    ImVec4 adjustedClear = clearColor * backgroundBrightness;

    gfx->clear(glm::vec4(
        adjustedClear.x,
        adjustedClear.y,
        adjustedClear.z,
        adjustedClear.w
    ));

    glm::mat4 modelA = glm::translate(glm::mat4(1.0f), glm::vec3(moveXA, moveYA, 0.0f));
    modelA = glm::rotate(modelA, angleA, glm::vec3(0, 0, 1));

    glm::mat4 modelB = glm::translate(glm::mat4(1.0f), glm::vec3(moveXB, moveYB, 0.0f));
    modelB = glm::rotate(modelB, angleB, glm::vec3(0, 0, 1));


    glm::vec4 color(
        1.0f * objectBrightness,
        r     * objectBrightness,
        0.2f  * objectBrightness,
        1.0f
    );
    gfx->draw(objA, modelA, color);
    gfx->draw(objB, modelB, color);
}

void App::renderImGui()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Hello, from ImGui");

    ImGui::Text("Object A Controls");
    ImGui::SliderFloat("Move X A", &moveXA, -2.0f, 2.0f);
    ImGui::SliderFloat("Move Y A", &moveYA, -1.5f, 1.5f);
    ImGui::SliderFloat("Rotate Speed A", &speedA, -1.0f, 1.0f);

    ImGui::Separator();
    ImGui::Text("Object B Controls");
    ImGui::SliderFloat("Move X B", &moveXB, -2.0f, 2.0f);
    ImGui::SliderFloat("Move Y B", &moveYB, -1.5f, 1.5f);
    ImGui::SliderFloat("Rotate Speed B", &speedB, -1.0f, 1.0f);

    ImGui::ColorEdit3("clear color", (float*)&clearColor);
    ImGui::SliderFloat("Object Brightness", &objectBrightness, 0.0f, 2.0f);
    ImGui::SliderFloat("Background Brightness", &backgroundBrightness, 0.0f, 2.0f);

    if (ImGui::Button("Reset All"))
    {
        moveXA = -1.5f;
        moveYA = moveXB = moveYB = 0.0f;
        speedA = speedB = 0.0f;
        angleA = angleB = 0.0f;
        objectBrightness = 1.0f;
        backgroundBrightness = 1.0f;
        clearColor = ImVec4(0.71f, 0.74f, 0.76f, 1.00f);
    }

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void App::shutdown()
{
    delete gfx;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}
