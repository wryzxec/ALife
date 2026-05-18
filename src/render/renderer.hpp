#pragma once

#include "alife/alife.hpp"

class Renderer {
public:
    virtual ~Renderer() = default;
    virtual void render(const ALife& sim) = 0;
};
