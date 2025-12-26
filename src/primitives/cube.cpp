#include "cube.hpp"
#include "../App.hpp"
#include "../vendor/glm/glm.hpp"
#include "../vendor/glm/gtc/matrix_transform.hpp"
void Cube::build(std::vector<ScreenObjeect>& out,
                 Graphicsengine& gfx)
{
    float s = 0.2f;

    auto addFace = [&](const glm::mat4& model, const glm::vec4& color)
    {
        ScreenObjeect o;
        o.id = gfx.createCube("res/textures/codethakur.png"); 
        o.model = model;
        o.color = color;
        o.isCubeFace = true;
        out.push_back(o);
    };

    // FRONT
    addFace(glm::translate(glm::mat4(1.0f), {0, 0, s}),
            {0, 0, 0, 1});

    // BACK
    addFace(glm::translate(
                glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), {0, 1, 0}),
                {0, 0, s}),
            {0, 0, 0, 1});

    // LEFT
    addFace(glm::translate(
                glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), {0, 1, 0}),
                {0, 0, s}),
            {0, 0, 0, 1});

    // RIGHT
    addFace(glm::translate(
                glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), {0, 1, 0}),
                {0, 0, s}),
            {0, 0, 0, 1});

    // TOP
    addFace(glm::translate(
                glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), {1, 0, 0}),
                {0, 0, s}),
            {0, 0, 0, 1});

    // BOTTOM
    addFace(glm::translate(
                glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), {1, 0, 0}),
                {0, 0, s}),
            {0, 0, 0, 1});
}
