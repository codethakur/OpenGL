#include "ImGuiUI.hpp"
#include "vendor/imgui/imgui.h"
#include "App.hpp"

void UIWindow::render()
{
    if (!open)
        return; 
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize, ImGuiCond_Always);
    ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoTitleBar ;
        //ImGuiWindowFlags_NoDocking;

    if (ImGui::Begin(m_title.c_str(), &open, flags))
    {

        ImGui::SliderFloat("Music Volume", &app.musicVolume, 0.0f, 1.0f);
        Mix_VolumeMusic(static_cast<int>(app.musicVolume * 128.0f));
        ImGui::Separator();

        for (auto &c : children)
            c->render();
    }

    ImGui::End();
}

void UIObjectListPanel::render()
{
    if (ImGui::Button("Cube"))
    {
        if (onAddObject)
            onAddObject();
    }

   if (!controls || controls->empty())
    return;

    ImGui::Separator();

    for (size_t i = 0; i < controls->size(); ++i)
    {
        std::string title = "Object " + std::to_string(i + 1) + " Controls";
        ImGui::Text("%s", title.c_str());

        ImGui::SliderFloat(("Move X##" + std::to_string(i)).c_str(),
                        &(*controls)[i].moveX, -1.3120f, 1.312f);

        ImGui::SliderFloat(("Move Y##" + std::to_string(i)).c_str(),
                        &(*controls)[i].moveY, -0.853f, 0.853f);

        ImGui::SliderFloat(("Rotate Speed##" + std::to_string(i)).c_str(),
                        &(*controls)[i].rotatespeed, -1.0f, 1.0f);

        ImGui::Separator();
    }


    ImGui::SliderFloat(
        "Object Brightness",
        objectBrightness,
        0.0f, 2.0f);

    ImGui::SliderFloat(
        "Background Brightness",
        backgroundBrightness,
        0.0f, 2.0f);
    ImGui::Separator();
    ImGui::ColorEdit3("clear color", (float *)clearColor);

    if (ImGui::Button("Reset All"))
    {
        if (onResetAll)
            onResetAll();
    }
}