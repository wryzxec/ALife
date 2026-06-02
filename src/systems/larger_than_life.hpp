#pragma once

#include <memory>
#include <utility>

#include "alife/observations/neighbour.hpp"
#include "alife/rules/larger_than_life.hpp"
#include "alife/integrators/discrete.hpp"
#include "alife/constraints/binary.hpp"
#include "alife/alife.hpp"

class LargerThanLife : public ALife {
public:
    LargerThanLife(
            State initialState, size_t radius,
            int birthMin, int birthMax, int surviveMin, int surviveMax)
        : ALife (
            std::move(initialState),
            std::make_unique<NbrObservation>(radius),
            std::make_unique<LargerThanLifeRule>(
                birthMin, birthMax, surviveMin, surviveMax
            ),
            std::make_unique<DiscreteIntegrator>(),
            std::make_unique<BinaryConstraint>()
        )
    {}
};
