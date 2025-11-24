#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Graphicsengine.hpp"
#include "vendor/imgui/imgui.h"

class App {
    struct SceneObject {
    Graphicsengine::ObjectId id;
    glm::mat4 model;
    glm::vec4 color;
};

std::vector<SceneObject> objects;

public:
    App();
    ~App();

    void run();

private:
    GLFWwindow* window = nullptr;
    Graphicsengine* gfx = nullptr;

    Graphicsengine::ObjectId objA = 0;
    Graphicsengine::ObjectId objB = 0;

    void initWindow();
    void initGL();
    void initImGui();
    void loadResources();

    void update();
    void render();
    void renderImGui();
    void shutdown();

    float angleA = 0.0f, angleB = 0.0f;
    float moveXA = -1.5f, moveYA = 0.0f;
    float moveXB = 0.0f, moveYB = 0.0f;
    float speedA = 0.0f, speedB = 0.0f;
    float r = 0.0f, increment = 0.05f;

    
    float objectBrightness = 1.0f;
    float backgroundBrightness = 1.0f;

    ImVec4 clearColor = ImVec4(0.71f, 0.74f, 0.76f, 1.00f);

};
