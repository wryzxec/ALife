#pragma once

#include <memory>
#include <utility>

#include "alife/observations/nbr_observation.hpp"
#include "alife/rules/conway_rule.hpp"
#include "alife/integrators/discrete_integrator.hpp"
#include "alife/constraints/binary_constraint.hpp"
#include "alife/alife.hpp"

class Conway : public ALife {
public:
    Conway(State initialState)
        : ALife (
            std::move(initialState),
            std::make_unique<NbrObservation>(),
            std::make_unique<ConwayRule>(),
            std::make_unique<DiscreteIntegrator>(),
            std::make_unique<BinaryConstraint>()
        )
    {}
};
