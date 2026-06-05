#include <cstddef>
#include <memory>
#include <utility>

#include "app.hpp"
#include "config/sim_preset_loader.hpp"
#include "core/state.hpp"
#include "patterns/pattern_loader.hpp"
#include "patterns/pattern_preset.hpp"
#include "render/sfml_renderer.hpp"
#include "systems/lenia/lenia.hpp"

int main() {
    SimulationPreset simPreset = loadSimulationPreset("assets/presets/lenia/fish.json");
    State initialState(simPreset.world.rows, simPreset.world.cols, simPreset.world.channels);
    PatternPreset pattern = loadPatternPreset(simPreset.patternPath.value(), simPreset.spatialScale);
    initialState.placePatternCentred(pattern.pattern);

    auto sim = std::make_unique<Lenia>(
        std::move(initialState),
        interactionsFromConfig(simPreset.lenia.value()),
        simPreset.world.rows,
        simPreset.world.cols,
        simPreset.lenia->dt,
        constraintFromMode(simPreset.lenia->constraintMode)
    );

    auto renderer = std::make_unique<SFMLRenderer>(
        simPreset.world.rows,
        simPreset.world.cols,
        2
    );

    App app(std::move(sim), std::move(renderer));
    app.run();

    return 0;
}
