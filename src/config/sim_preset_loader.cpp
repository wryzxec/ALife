#include <cstddef>
#include <fstream>
#include <stdexcept>
#include <string>

#include <nlohmann/json.hpp>

#include "config/sim_preset_loader.hpp"
#include "config/sim_preset.hpp"

using json = nlohmann::json;

namespace {

WorldConfig loadWorldConfig(const json& data) {
    return WorldConfig{
        data.at("rows").get<size_t>(),
        data.at("cols").get<size_t>(),
        data.at("channels").get<size_t>(),
    };
};

KernelConfig loadKernelConfig(const json& data, size_t spatialScale) {
    KernelConfig config;

    config.type = data.at("type").get<std::string>();
    config.radius = data.at("radius").get<double>() * spatialScale;

    config.mu = data.value("mu", 0.5);
    config.sigma = data.value("sigma", 0.15);
    config.alpha = data.value("alpha", 4.0);

    if (data.contains("beta")) {
        config.beta = data.at("beta").get<std::vector<double>>();
    }

    return config;
}

GrowthConfig loadGrowthConfig(const json& data) {
    return GrowthConfig{
        data.at("mu").get<double>(),
        data.at("sigma").get<double>()
    };
}

LargerThanLifeConfig loadLargerThanLifeConfig(const json& data) {
    return LargerThanLifeConfig{
        data.at("radius").get<std::size_t>(),

        data.at("birthMin").get<int>(),
        data.at("birthMax").get<int>(),

        data.at("surviveMin").get<int>(),
        data.at("surviveMax").get<int>()
    };
}

SmoothLifeConfig loadSmoothLifeConfig(const json& data) {
    return SmoothLifeConfig{
        data.at("radius").get<std::size_t>(),

        data.at("birthMin").get<double>(),
        data.at("birthMax").get<double>(),

        data.at("surviveMin").get<double>(),
        data.at("surviveMax").get<double>(),

        data.at("alphaN").get<double>(),
        data.at("alphaM").get<double>(),

        data.at("dt").get<double>()
    };
}

UpdateMode updateModeFromString(const std::string& str) {
    if(str == "classic") {
        return UpdateMode::Classic;
    }

    if(str == "asymptotic") {
        return UpdateMode::Asymptotic;
    }

    throw std::invalid_argument("Unknown Lenia update mode: " + str);
}

InteractionConfig loadInteractionConfig(const json& data, size_t spatialScale) {
    return InteractionConfig{
        data.at("source").get<size_t>(),
        data.at("target").get<size_t>(),
        data.at("weight").get<double>(),
        updateModeFromString(data.value("update", "classic")),
        loadKernelConfig(data.at("kernel"), spatialScale),
        loadGrowthConfig(data.at("growth"))
    };
}

std::vector<InteractionConfig> loadInteractionConfigs(const json& data, size_t spatialScale) {
    std::vector<InteractionConfig> interactions;
    interactions.reserve(data.size());

    for(const auto& interaction : data) {
        interactions.push_back(loadInteractionConfig(interaction, spatialScale));
    }

    return interactions;
}

ConstraintMode constraintModeFromString(const std::string& str) {
    if(str == "clamp") return ConstraintMode::Clamp;
    if(str == "sigmoid") return ConstraintMode::Sigmoid;
    
    throw std::invalid_argument("Unknown constraint mode: " + str);
}

LeniaConfig loadLeniaConfig(const json& data, double spatialScale) {
    return LeniaConfig{
        data.at("dt").get<double>(),
        constraintModeFromString(data.value("constraint", "clamp")),
        loadInteractionConfigs(data.at("interactions"), spatialScale)
    };
}

}

SimulationPreset loadSimulationPreset(const std::filesystem::path& path) {
    std::ifstream file(path);

    if(!file) {
        throw std::runtime_error("Could not open simulation preset file: " + path.string());
    }

    json data;
    file >> data;

    SimulationPreset preset;

    preset.name = data.value("name", "");
    preset.system = systemTypeFromString(data.at("system").get<std::string>());
    preset.desc = data.value("desc", "");

    preset.world = loadWorldConfig(data.at("world"));

    preset.spatialScale = data.value("spatial_scale", size_t{1});

    if (data.contains("pattern")) {
        preset.patternPath = data.at("pattern").get<std::string>();
    }

    if(data.contains("parameters")) {
        const json& parameters = data.at("parameters");

        switch (preset.system) {
            case SystemType::Conway:
                break;

            case SystemType::Lenia:
                preset.lenia = loadLeniaConfig(parameters, preset.spatialScale);
                break;

            case SystemType::LargerThanLife:
                preset.largerThanLife =
                    loadLargerThanLifeConfig(parameters);
                break;

            case SystemType::SmoothLife:
                preset.smoothLife =
                    loadSmoothLifeConfig(parameters);
                break;
        }
    }

    return preset;
}
