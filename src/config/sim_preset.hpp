#pragma once

#include <cstddef>
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

#include "systems/system_type.hpp"

struct WorldConfig {
    size_t rows;
    size_t cols;
    size_t channels = 1;
};

struct KernelConfig {
    std::string type;
    size_t radius;
    double mu;
    double sigma;

    double alpha;
    std::vector<double> beta;
};

struct GrowthConfig {
    double mu;
    double sigma;
};

struct LargerThanLifeConfig {
    size_t radius;

    int birthMin;
    int birthMax;

    int surviveMin;
    int surviveMax;
};

struct SmoothLifeConfig {
    size_t radius;

    double birthMin;
    double birthMax;

    double surviveMin;
    double surviveMax;

    double alphaN;
    double alphaM;

    double dt;
};

struct LeniaConfig {
    KernelConfig kernel;
    GrowthConfig growth;
    double dt = 1.0;
};

struct SimulationPreset {
    std::string name;
    SystemType system;
    std::string desc;

    WorldConfig world;
    std::optional<std::filesystem::path> patternPath;
    std::optional<LargerThanLifeConfig> largerThanLife;
    std::optional<SmoothLifeConfig> smoothLife;
    std::optional<LeniaConfig> lenia;
};
