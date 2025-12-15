#define IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_IMPL_OPENGL_LOADER_GLAD

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <memory>

#include "App.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw.h"
#include "vendor/imgui/imgui_impl_opengl3.h"
#include "Console.hpp"
#include "ImGuiUI.hpp"

App::App()
{
    initWindow();
    initGL();
    initImGui();       
    initImGuiWindow();
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

        renderImGuiWindow();

        glfwSwapBuffers(window);
    }
    shutdownImGuiWindow();
}

void App::initWindow()
{
    if (!glfwInit())
    {
        Console::LOGN("Failed to init GLFW", Color::RED);
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_CORE_PROFILE, GLFW_TRUE);

    window = glfwCreateWindow(940, 680, "OpenGL Window", NULL, NULL);
    if (!window)
    {
        Console::LOGN("Failed to create GLFW main window", Color::RED);
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
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
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
    ImGui::StyleColorsDark();

    mainImGuiContext = ImGui::GetCurrentContext();

    uiRoot = std::make_shared<UIWindow>("Object Editor");
    auto objPanel = std::make_shared<UIObjectListPanel>(&objects, &controls, &clearColor, &objectBrightness, &backgroundBrightness);


    objPanel->onAddObject = [this]()
    {
        ScreenObjeect o;
        o.id = gfx->createQuad("res/textures/codethakur.png");
        o.model = glm::mat4(1.0f);
        o.color = glm::vec4(1.0f);
        objects.push_back(o);

        ObjectControl c;
        const float offset = 0.5f * float(objects.size() - 1);
        c.moveX = -1.5f + offset;
        c.moveY = 0.0f;
        c.rotatespeed = 0.0f;
        c.angle = 0.0f;
        controls.push_back(c);
    };

    objPanel->onRemoveLast = [this]()
    {
        if (!objects.empty()  && objects.size()>1) {
            objects.pop_back();
            controls.pop_back();
        }
    };

    objPanel->onResetAll = [this]()
    {
        for (size_t i = 0; i < controls.size(); ++i) {
            controls[i].moveX = (i == 0 ? -1.5f : 0.0f);
            controls[i].moveY = 0.0f;
            controls[i].rotatespeed = 0.0f;
            controls[i].angle = 0.0f;

            objects[i].color = glm::vec4(1.0f);
            objects[i].model = glm::mat4(1.0f);
            
        }

        objectBrightness = 1.0f;
        backgroundBrightness = 1.0f;
        clearColor = ImVec4(0.71f, 0.74f, 0.76f, 1.00f);
    };

    uiRoot->add(objPanel);
}

void App::initImGuiWindow()
{
    imguiWindow = glfwCreateWindow(450, 520, "Controls Window", nullptr, window);
    if (!imguiWindow)
    {
        Console::LOGN("Failed to create ImGui Controls window", Color::RED);
        return;
    }
    glfwMakeContextCurrent(imguiWindow);

    ImGui::SetCurrentContext(mainImGuiContext);

    ImGui_ImplGlfw_InitForOpenGL(imguiWindow, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    glfwMakeContextCurrent(window);
    ImGui::SetCurrentContext(mainImGuiContext);
}

void App::renderImGuiWindow()
{
    if (!imguiWindow) return;

    if (glfwWindowShouldClose(imguiWindow))
    {
        shutdownImGuiWindow();
        return;
    }

    glfwMakeContextCurrent(imguiWindow);
    ImGui::SetCurrentContext(mainImGuiContext);
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (uiRoot)
        uiRoot->render();

    ImGui::Render();

    int w, h;
    glfwGetFramebufferSize(imguiWindow, &w, &h);
    glViewport(0, 0, w, h);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(imguiWindow);
    glfwMakeContextCurrent(window);
    ImGui::SetCurrentContext(mainImGuiContext);
}
void App::loadResources()
{
    gfx = new Graphicsengine(window);
    objects.clear();
    controls.clear();

    ScreenObjeect screenObj;
    screenObj.id = gfx->createQuad("res/textures/codethakur.png");
    screenObj.model = glm::mat4(1.0f);
    screenObj.color = glm::vec4(1.0f);
    objects.push_back(std::move(screenObj));

    ObjectControl objControl;
    objControl.moveX = -1.5f;
    objControl.moveY = 0.0f;
    objControl.rotatespeed = 0.0f;
    objControl.angle = 0.0f;
    controls.push_back(objControl);

    r = 0.0f;
    increment = 0.05f;
    objectBrightness = 1.0f;
    backgroundBrightness = 1.0f;
}

glm::vec2 App::screenToWorld(double mx, double my)
{
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    float x = (mx / width) * 2.0f - 1.0f;
    float y = 1.0f - (my / height) * 2.0f;

    glm::vec4 world = glm::inverse(gfx->proj) * glm::vec4(x, y, 0.0f, 1.0f);
    return glm::vec2(world.x, world.y);
}

void App::update()
{
    ImGui::SetCurrentContext(mainImGuiContext);
    ImGuiIO io = ImGui::GetIO();
    if (io.WantCaptureMouse) return;

    double mx, my;
    glfwGetCursorPos(window, &mx, &my);
    glm::vec2 mouseWorld = screenToWorld(mx, my);

    bool mouseDown = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;

    const float halfW = 0.5f;
    const float halfH = 0.5f;

    if (!dragging && mouseDown)
    {
        for (int i = int(controls.size()) - 1; i >= 0; --i)
        {
            float x = controls[i].moveX;
            float y = controls[i].moveY;

            if (mouseWorld.x > x - halfW && mouseWorld.x < x + halfW &&
                mouseWorld.y > y - halfH && mouseWorld.y < y + halfH)
            {
                dragging = true;
                draggedIndex = i;
                dragOffset = mouseWorld - glm::vec2(x, y);
                break;
            }
        }
    }

    if (dragging)
    {
        if (!mouseDown)
        {
            dragging = false;
            draggedIndex = -1;
        }
        else
        {
            controls[draggedIndex].moveX = mouseWorld.x - dragOffset.x;
            controls[draggedIndex].moveY = mouseWorld.y - dragOffset.y;
        }
    }

    if (r > 1.0f) increment = -0.05f;
    else if (r < 0.0f) increment = 0.05f;
    r += increment;

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    float aspect = (width > 0 && height > 0) ? (float)width / (float)height : 1.0f;
    float orthoHeight = 6.0f / aspect;

    const float left   = -3.0f + halfW;
    const float right  =  3.0f - halfW;
    const float bottom = -orthoHeight / 2.0f + halfH;
    const float top    =  orthoHeight / 2.0f - halfH;

    for (size_t i = 0; i < controls.size(); i++)
    {
        controls[i].moveX = std::clamp(controls[i].moveX, left, right);
        controls[i].moveY = std::clamp(controls[i].moveY, bottom, top);

        controls[i].angle += controls[i].rotatespeed;

        objects[i].model =
            glm::translate(glm::mat4(1.0f), glm::vec3(controls[i].moveX, controls[i].moveY, 0.0f));
        objects[i].model =
            glm::rotate(objects[i].model, controls[i].angle, glm::vec3(0, 0, 1));

        objects[i].color = glm::vec4(
            1.0f * objectBrightness,
            r     * objectBrightness,
            0.2f  * objectBrightness,
            1.0f
        );
    }
}

void App::render()
{
    ImVec4 adjustedClear = clearColor * backgroundBrightness;
    gfx->clear(glm::vec4(adjustedClear.x, adjustedClear.y, adjustedClear.z, adjustedClear.w));

    for (auto& obj : objects) {
        gfx->draw(obj.id, obj.model, obj.color);
    }
    
}

void App::shutdownImGuiWindow()
{
    if (!imguiWindow) return;
    glfwMakeContextCurrent(imguiWindow);
    ImGui::SetCurrentContext(mainImGuiContext);
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();

    glfwDestroyWindow(imguiWindow);
    imguiWindow = nullptr;

    glfwMakeContextCurrent(window);
    ImGui::SetCurrentContext(mainImGuiContext);
}

void App::shutdown()
{
    delete gfx;

    shutdownImGuiWindow();
    if (mainImGuiContext)
    {
        ImGui::SetCurrentContext(mainImGuiContext);
        ImGui::DestroyContext(mainImGuiContext);
        mainImGuiContext = nullptr;
    }

    if (window)
    {
        glfwDestroyWindow(window);
        window = nullptr;
    }

    glfwTerminate();
}
