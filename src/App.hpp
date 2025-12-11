#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include<vector>

#include "Graphicsengine.hpp"
#include "vendor/imgui/imgui.h"

class App {
public:
    struct ScreenObjeect
    {
        Graphicsengine::ObjectId id;
        glm::mat4 model;
        glm::vec4 color;
    };
    struct ObjectControl
    {
        float moveX = 0.0f;
        float moveY = 0.0f; 
        float rotatespeed = 0.0f;
        float angle = 0.0f;
    };
    std::vector<ScreenObjeect>objects;
    std::vector<ObjectControl>controls;
    
    App();
    ~App();
    bool dragging = false;
    int draggedIndex = -1;
    glm::vec2 dragOffset;
    void run();

private:
    GLFWwindow* window = nullptr;
    Graphicsengine* gfx = nullptr;


    void initWindow();
    void initGL();
    void initImGui();
    void loadResources();

    void update();
    void render();
    void renderImGui();
    void shutdown();

   

    float r = 0.0f, increment = 0.05f;

    
    float objectBrightness = 1.0f;
    float backgroundBrightness = 1.0f;

    ImVec4 clearColor = ImVec4(0.71f, 0.74f, 0.76f, 1.00f);
    glm::vec2 screenToWorld(double mx, double my);
    
};
