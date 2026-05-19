#pragma once

#include "alife/alife.hpp"

class Renderer {
public:
    virtual ~Renderer() = default;
    virtual void render(const ALife& sim) = 0;
    virtual void handleEvents() = 0;
    virtual bool isOpen() const = 0;
};
