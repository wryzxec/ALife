#pragma once

#include <memory>
#include <utility>

#include "alife/observations/nbr_observation.hpp"
#include "alife/rules/larger_than_life_rule.hpp"
#include "alife/integrators/discrete_integrator.hpp"
#include "alife/constraints/binary_constraint.hpp"
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
