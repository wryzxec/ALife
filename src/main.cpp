#include <memory>

#include "app.hpp"
#include "core/state.hpp"
#include "systems/conway.hpp"
#include "systems/larger_than_life.hpp"
#include "render/sfml_renderer.hpp"

int main() {
    State initialState(150, 150, 1);
    initialState.randomiseBinary(0.4);
    
    // auto sim = std::make_unique<Conway>(std::move(initialState));
    auto sim = std::make_unique<LargerThanLife>(
            std::move(initialState),
            5,
            35, 45,
            34, 58
    );
    auto renderer = std::make_unique<SFMLRenderer>(200, 200, 5);

    App app(std::move(sim), std::move(renderer));
    app.run();
    return 0;
}
