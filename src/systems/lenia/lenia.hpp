#pragma once

#include <cstddef>
#include <memory>
#include <utility>
#include <vector>

#include "alife/alife.hpp"
#include "alife/constraint.hpp"
#include "alife/constraints/clamp.hpp"
#include "alife/observations/channel_convolution.hpp"
#include "alife/integrators/euler.hpp"
#include "alife/rules/lenia.hpp"
#include "systems/lenia/interaction.hpp"

class Lenia : public ALife {
public:
    Lenia(
            State initialState, std::vector<Interaction> interactions,
            size_t rows, size_t cols, double dt,
            std::unique_ptr<Constraint> constraint = std::make_unique<ClampConstraint>(0.0, 1.0)
    )
        : ALife(
            std::move(initialState),
            std::make_unique<ChannelConvObservation>(interactions, rows, cols),
            std::make_unique<LeniaRule>(interactions),
            std::make_unique<EulerIntegrator>(dt),
            std::move(constraint)
        )
    {}
};
