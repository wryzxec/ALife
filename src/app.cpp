#include <thread>
#include <chrono>
#include <utility>

#include "app.hpp"

App::App(
    std::unique_ptr<ALife> sim,
    std::unique_ptr<Renderer> renderer
)
    : _simulation{std::move(sim)},
      _renderer{std::move(renderer)}
{}

void App::run() {
    while(_renderer->isOpen()) {
        _renderer->handleEvents();
        _simulation->step();
        _renderer->render(*_simulation);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}
