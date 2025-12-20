#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include<vector>
#include "Graphicsengine.hpp"
#include "vendor/imgui/imgui.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

struct ScreenObjeect
{
    Graphicsengine::ObjectId id;
    glm::mat4 model;
    glm::vec4 color;
    bool isCubeFace = false; 
};

struct ObjectControl
{
    float moveX = 0.0f;
    float moveY = 0.0f;
    float rotatespeed = 0.0f;
    float angle = 0.0f;
    
};

class UIWindow; 

class App {
public:
    App();
    ~App();
    void run();

    std::vector<ScreenObjeect> objects;
    std::vector<ObjectControl> controls;

private:
    void initAudio();
    void shutdownAudio();

    Mix_Music* backgroundMusic = nullptr;
   
    GLFWwindow* window = nullptr;           
    GLFWwindow* imguiWindow = nullptr;      
    ImGuiContext* mainImGuiContext = nullptr; 

    std::shared_ptr<UIWindow> uiRoot;
    Graphicsengine* gfx = nullptr;

    void initWindow();
    void initGL();
    void initImGui();        
    void initImGuiWindow(); 
    void loadResources();

    void update();
    void render();
    void renderImGuiWindow();  

    void shutdownImGuiWindow(); 
    void shutdown();

    glm::vec2 screenToWorld(double mx, double my);

    bool dragging = false;
    int draggedIndex = -1;
    glm::vec2 dragOffset;
    glm::mat4 cubeTransform = glm::mat4(1.0f);
    float objectBrightness = 1.0f;
    float backgroundBrightness = 1.0f;
    float r = 0.0f, increment = 0.05f;
    ImVec4 clearColor = ImVec4(0.71f, 0.74f, 0.76f, 1.00f);
    


};