#pragma once

#include <memory>
#include <utility>

#include "alife/observations/neighbour.hpp"
#include "alife/rules/conway.hpp"
#include "alife/integrators/discrete.hpp"
#include "alife/constraints/binary.hpp"
#include "alife/alife.hpp"

class Conway : public ALife {
public:
    Conway(State initialState)
        : ALife (
            std::move(initialState),
            std::make_unique<NbrObservation>(1),
            std::make_unique<ConwayRule>(),
            std::make_unique<DiscreteIntegrator>(),
            std::make_unique<BinaryConstraint>()
        )
    {}
};
