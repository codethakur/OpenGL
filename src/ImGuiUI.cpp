#include "ImGuiUI.hpp"
#include "vendor/imgui/imgui.h"
#include "App.hpp"

void UIWindow::render()
{
    if (!open) return;

    ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoTitleBar;

    if (ImGui::Begin(m_title.c_str(), &open, flags))
    {
        for (auto& c : children)
            c->render();
    }

    ImGui::End();
}


void UIObjectListPanel::render()
{
    if (!objects || !controls) return;

    if (ImGui::Button("More Object"))
    {
        if (onAddObject) onAddObject();
    }

    ImGui::SameLine();
    if(objects->size()>1){
        if (ImGui::Button("Remove  Object"))
        {
            if (onRemoveLast) onRemoveLast();
        }
    }

    ImGui::Separator();

    for (size_t i = 0; i < controls->size(); ++i)
    {
        std::string title = "Object " + std::to_string(i + 1) + " Controls";
        ImGui::Text("%s", title.c_str());

        ImGui::SliderFloat(("Move X##" + std::to_string(i)).c_str(),
                           &(*controls)[i].moveX, -1.2120f, 1.212f);
        ImGui::SliderFloat(("Move Y##" + std::to_string(i)).c_str(),
                           &(*controls)[i].moveY, -0.753f, 0.753f);
        ImGui::SliderFloat(("Rotate Speed##" + std::to_string(i)).c_str(),
                           &(*controls)[i].rotatespeed, -1.0f, 1.0f);

        ImGui::SameLine();
        if (ImGui::Button(("Reset##" + std::to_string(i)).c_str()))
        {
            (*controls)[i].moveX = (i == 0 ? -1.5f : 0.0f);
            (*controls)[i].moveY = 0.0f;
            (*controls)[i].rotatespeed = 0.0f;
            (*controls)[i].angle = 0.0f;

            (*objects)[i].model = glm::mat4(1.0f);
           
        }

        ImGui::Separator();
    }
    ImGui::SliderFloat(
    "Object Brightness",
    objectBrightness,  
    0.0f, 2.0f
);

ImGui::SliderFloat(
    "Background Brightness",
    backgroundBrightness, 
    0.0f, 2.0f
);
ImGui::Separator();
ImGui::ColorEdit3("clear color", (float*)clearColor);

    if (ImGui::Button("Reset All"))
    {
        if (onResetAll) onResetAll();
    }
}