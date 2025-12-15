#pragma once
#include <vector>
#include <memory>
#include <string>
#include <functional>
#include "vendor/imgui/imgui.h"
#include "App.hpp" 


class UIComponent {
public:
    virtual ~UIComponent() = default;
    virtual void render() = 0;
    virtual void add(std::shared_ptr<UIComponent>) {}
};

class UIWindow : public UIComponent {
public:
    std::string m_title;
    bool open = true;
    UIWindow(const std::string& title) : m_title(title) {}
    void add(std::shared_ptr<UIComponent> c) override { children.push_back(c); }
    void render() override;
private:
    std::vector<std::shared_ptr<UIComponent>> children;
};
class UIObjectListPanel : public UIComponent {
public:

    UIObjectListPanel(std::vector<ScreenObjeect>* objs, std::vector<ObjectControl>* ctrls,  ImVec4* clr, float* objBrightness, float* bgBrightness)
        : objects(objs), controls(ctrls),objectBrightness(objBrightness), backgroundBrightness(bgBrightness),clearColor(clr){}
    
    void render() override;

    std::function<void()> onAddObject;
    std::function<void()> onRemoveLast;
    std::function<void()> onResetAll;


private:
    std::vector<ScreenObjeect>* objects;
    std::vector<ObjectControl>* controls;
    float* objectBrightness;       
    float* backgroundBrightness;   
    ImVec4* clearColor;
};