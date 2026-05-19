#include <memory>

#include "app.hpp"
#include "core/state.hpp"
#include "systems/conway.hpp"
#include "render/sfml_renderer.hpp"

int main() {
    State initialState(50, 50, 1);
    initialState(1, 2) = 1.0;
    initialState(2, 3) = 1.0;
    initialState(3, 1) = 1.0;
    initialState(3, 2) = 1.0;
    initialState(3, 3) = 1.0;
    
    auto sim = std::make_unique<Conway>(std::move(initialState));
    auto renderer = std::make_unique<SFMLRenderer>(50, 50);

    App app(std::move(sim), std::move(renderer));
    app.run();
    return 0;
}
