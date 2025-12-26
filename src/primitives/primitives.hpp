#pragma once
#include <vector>

class Graphicsengine;
struct ScreenObjeect;

struct Primitive {
    virtual ~Primitive() = default;
    virtual void build(std::vector<ScreenObjeect>& out,
                       Graphicsengine& gfx) = 0;
};
