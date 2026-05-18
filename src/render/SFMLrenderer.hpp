#pragma once

#include "render/renderer.hpp"

class SFMLRenderer : public Renderer {
public:
    void render(const ALife& sim) override;
};
