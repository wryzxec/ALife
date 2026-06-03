#pragma once

#include <cstddef>
#include <utility>

#include "core/kernel.hpp"

struct Interaction {
    size_t sourceChannel = 0;
    size_t targetChannel = 0;

    Kernel kernel;

    double growthMu;
    double growthSigma;

    double weight = 1.0;

    Interaction(
            size_t source, size_t target,
            Kernel kernel_,
            double mu, double sigma,
            double weight_ = 1.0
    )
        : sourceChannel{source}, targetChannel{target}, 
          kernel{std::move(kernel_)},
          growthMu{mu}, growthSigma{sigma}, weight{weight_}
    {}
};
