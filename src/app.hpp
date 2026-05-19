#pragma once

#include <memory>

#include "alife/alife.hpp"
#include "render/renderer.hpp"

class App {
public:
    App(
        std::unique_ptr<ALife> sim,
        std::unique_ptr<Renderer> renderer
    );

    void run();
    void stop();
private:
    bool _running;
    std::unique_ptr<ALife> _simulation;
    std::unique_ptr<Renderer> _renderer;
};
