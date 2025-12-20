#define IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_IMPL_OPENGL_LOADER_GLAD

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <memory>
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw.h"
#include "vendor/imgui/imgui_impl_opengl3.h"
#include "Console.hpp"
#include "ImGuiUI.hpp"
#include "App.hpp"


App::App()
{
    initWindow();
    initGL();
    initAudio(); 
    initImGui();
    initImGuiWindow();
    loadResources();
}

App::~App()
{
    shutdownAudio();
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
}

void App::initWindow()
{
    if (!glfwInit())
    {
        std::cerr << "GLFW init failed\n";
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_CORE_PROFILE, GLFW_TRUE);
    
    App::window = glfwCreateWindow(940, 680, "OpenGL Window", NULL, NULL);
    if (!window)
    {
        Console::LOGN("Failed to create GLFW main window", Color::RED);
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glfwSetFramebufferSizeCallback(window, [](GLFWwindow *, int w, int h)
                                   { glViewport(0, 0, w, h); });
}

void App::initGL()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        Console::LOGN("Failed to init GLAD", Color::RED);
        return;
    }

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
}

void App::initImGui()
{

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    mainImGuiContext = ImGui::GetCurrentContext();

    uiRoot = std::make_shared<UIWindow>("Object Editor", *this);
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
        if (!objects.empty() && objects.size() > 1)
        {
            objects.pop_back();
            controls.pop_back();
        }
    };

    objPanel->onResetAll = [this]()
    {
        for (size_t i = 0; i < controls.size(); ++i)
        {
            controls[i].moveX = (i == 0 ? -0.0f : 0.0f);
            controls[i].moveY = 0.0f;
            controls[i].rotatespeed = 0.0f;
            controls[i].angle = 0.0f;
        }

        objectBrightness = 1.0f;
        backgroundBrightness = 1.0f;
        clearColor = ImVec4(0.71f, 0.74f, 0.76f, 1.00f);
    };

    uiRoot->add(objPanel);
}

void App::initImGuiWindow()
{
    imguiWindow = glfwCreateWindow(550, 200, "Controls Window", nullptr, window);
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
    if (!imguiWindow)
        return;

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
    r += increment;
    if (r > 1.0f || r < 0.0f)
        increment = -increment;
    float s = 0.2;

    auto addFace = [&](const glm::mat4 &model, const glm::vec4 &color)
    {
        ScreenObjeect o;
        o.id = gfx->createQuad("res/textures/codethakur.png");
        o.model = model;
        o.color = color;
        o.isCubeFace = true;
        objects.push_back(o);
    };
    // FRONT
    addFace(
        glm::translate(glm::mat4(1.0f), {0, 0, s}),
         {0, 0, 0, 1});

    // BACK
    addFace(
        glm::translate(
            glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), {0, 1, 0}),
            {0, 0, s}),
        {0, 0, 0, 1});

    // LEFT
    addFace(
        glm::translate(
            glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), {0, 1, 0}),
            {0, 0, s}),
        {0, 0, 0, 1});

    // RIGHT
    addFace(
        glm::translate(
            glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), {0, 1, 0}),
            {0, 0, s}),
        {0, 0, 0, 1});

    // TOP
    addFace(
        glm::translate(
            glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), {1, 0, 0}),
            {0, 0, s}),
        {0, 0, 0, 1});

    // BOTTOM
    addFace(
        glm::translate(
            glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), {1, 0, 0}),
            {0, 0, s}),
        {0, 0, 0, 1});

    r = 0.0f;
    increment = 0.05f;
    objectBrightness = 1.0f;
    backgroundBrightness = 1.0f;
    controls.clear();
    controls.resize(1);
}

void App::render()
{
    ImVec4 adjustedClear = clearColor * backgroundBrightness;
    int sdlVolume = static_cast<int>(musicVolume * MIX_MAX_VOLUME);
    Mix_VolumeMusic(sdlVolume);
    gfx->clear(glm::vec4(adjustedClear.x, adjustedClear.y, adjustedClear.z, adjustedClear.w));

    for (auto &obj : objects)
    {
        gfx->draw(obj.id, cubeTransform * obj.model, obj.color);
    }
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
    
    r += increment;
    if (r > 1.0f || r < 0.0f)
        increment = -increment;
    for (auto& o : objects)
    {
        if (o.isCubeFace)
            o.color = {1.0f, r, 0.0f, 1.0f};
    }
    ImGui::SetCurrentContext(mainImGuiContext);
    ImGuiIO io = ImGui::GetIO();
    if (io.WantCaptureMouse)
        return;

    double mx, my;
    glfwGetCursorPos(window, &mx, &my);
    glm::vec2 mouseWorld = screenToWorld(mx, my);

    bool mouseDown = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;

    const float cubeHalf = 0.5f;

    if (!dragging && mouseDown)
    {
        for (int i = int(controls.size()) - 1; i >= 0; --i)
        {
            float x = controls[i].moveX;
            float y = controls[i].moveY;

            if (mouseWorld.x > x - cubeHalf && mouseWorld.x < x + cubeHalf &&
                mouseWorld.y > y - cubeHalf && mouseWorld.y < y + cubeHalf)

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

    ObjectControl &c = controls[0];
    c.angle += c.rotatespeed;

    cubeTransform =
        glm::translate(glm::mat4(1.0f),
                       glm::vec3(c.moveX, c.moveY, 0.0f));

    cubeTransform =
        glm::rotate(cubeTransform,
                    c.angle,
                    glm::vec3(0, 1, 0));
}
void App::initAudio()
{
    SDL_InitSubSystem(SDL_INIT_AUDIO);

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    backgroundMusic = Mix_LoadMUS("res/gamemusic-6082.mp3");
    if (!backgroundMusic) {
        std::cout << "Audio load failed\n";
        return;
    }

    Mix_PlayMusic(backgroundMusic, -1); 
}
void App::shutdownAudio()
{
    if (backgroundMusic) {
        Mix_FreeMusic(backgroundMusic);
        backgroundMusic = nullptr;
    }

    Mix_CloseAudio();
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
}
void App::shutdownImGuiWindow()
{
    if (!imguiWindow)
        return;
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