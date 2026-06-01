#pragma once

#include "config/sim_preset.hpp"
#include "core/kernel.hpp"

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
