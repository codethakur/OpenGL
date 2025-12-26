#pragma once
#include "primitives.hpp"
#include "../Graphicsengine.hpp"

struct Cube : Primitive {
    void build(std::vector<ScreenObjeect>& out,
               Graphicsengine& gfx) override;
};
