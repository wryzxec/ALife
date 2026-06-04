#pragma once

#include <cstddef>
#include <utility>

#include "core/kernel.hpp"

enum class UpdateMode {
    Classic,
    Asymptotic
};

struct Interaction {
    size_t sourceChannel = 0;
    size_t targetChannel = 0;

    Kernel kernel;

    double growthMu;
    double growthSigma;

    double weight = 1.0;

    UpdateMode updateMode = UpdateMode::Classic;

    Interaction(
            size_t source, size_t target,
            Kernel kernel_,
            double mu, double sigma,
            double weight_ = 1.0,
            UpdateMode updateMode_ = UpdateMode::Classic
    )
        : sourceChannel{source}, targetChannel{target}, 
          kernel{std::move(kernel_)},
          growthMu{mu}, growthSigma{sigma}, weight{weight_},
          updateMode{updateMode_}
    {}
};
