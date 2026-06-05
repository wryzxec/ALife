#pragma once

#include <cstddef>
#include <filesystem>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "alife/constraint.hpp"
#include "alife/constraints/clamp.hpp"
#include "alife/constraints/sigmoid.hpp"
#include "core/kernel.hpp"
#include "systems/lenia/interaction.hpp"
#include "systems/system_type.hpp"

struct WorldConfig {
    size_t rows;
    size_t cols;
    size_t channels = 1;
};

struct KernelConfig {
    std::string type;
    double radius;
    double mu;
    double sigma;

    double alpha;
    std::vector<double> beta;
};

inline Kernel kernelFromConfig(const KernelConfig& config) {
    if (config.type == "gaussian_rings") {
        return Kernel::gaussianRings(
            config.radius,
            config.mu,
            config.sigma,
            config.beta
        );
    }

    if (config.type == "multi_ring") {
        return Kernel::multiRing(
            config.radius,
            config.beta,
            config.alpha
        );
    }

    if (config.type == "gaussian") {
        return Kernel::gaussian(
            config.radius,
            config.sigma
        );
    }

    if (config.type == "uniform_square") {
        return Kernel::uniformSquare(config.radius);
    }

    throw std::invalid_argument(
        "Unknown kernel type: " + config.type
    );
}

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

struct InteractionConfig {
    size_t source = 0;
    size_t target = 0;
    double weight = 1.0;
    UpdateMode updateMode = UpdateMode::Classic;

    KernelConfig kernel;
    GrowthConfig growth;
};

enum class ConstraintMode {
    Clamp,
    Sigmoid
};

inline std::unique_ptr<Constraint> constraintFromMode(ConstraintMode mode) {
    switch(mode) {
        case ConstraintMode::Clamp:
            return std::make_unique<ClampConstraint>(0.0, 1.0);

        case ConstraintMode::Sigmoid:
            return std::make_unique<SigmoidConstraint>();

        throw std::invalid_argument("Unkown constraint mode. ");
    }
}

struct LeniaConfig {
    double dt = 1.0;
    ConstraintMode constraintMode = ConstraintMode::Clamp;
    std::vector<InteractionConfig> interactions;
};

inline Interaction interactionFromConfig(const InteractionConfig& config) {
    return Interaction {
        config.source,
        config.target,
        kernelFromConfig(config.kernel),
        config.growth.mu,
        config.growth.sigma,
        config.weight,
        config.updateMode
    };
}

inline std::vector<Interaction> interactionsFromConfig(const LeniaConfig& config) {
    std::vector<Interaction> interactions;
    interactions.reserve(config.interactions.size());

    for(const InteractionConfig& interactionConfig : config.interactions) {
        interactions.push_back(interactionFromConfig(interactionConfig));
    }

    return interactions;
}

struct SimulationPreset {
    std::string name;
    SystemType system;
    std::string desc;

    WorldConfig world;

    double spatialScale = 1; // does not impact world size, just the scaling of the pattern, and kernels.

    std::optional<std::filesystem::path> patternPath;
    std::optional<LargerThanLifeConfig> largerThanLife;
    std::optional<SmoothLifeConfig> smoothLife;
    std::optional<LeniaConfig> lenia;
};


