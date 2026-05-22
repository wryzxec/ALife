#pragma once

#include <memory>
#include <utility>

#include "alife/alife.hpp"
#include "alife/constraints/clamp_constraint.hpp"
#include "alife/integrators/euler_integrator.hpp"
#include "alife/observations/smooth_observation.hpp"
#include "alife/rules/smoothlife_rule.hpp"

class SmoothLife : public ALife {
public:
    SmoothLife(
            State initialState, size_t radius,
            double birthMin, double birthMax, double surviveMin, double surviveMax,
            double alphaN, double alphaM, double dt
    )
        : ALife(
            std::move(initialState),
            std::make_unique<SmoothObservation>(radius),
            std::make_unique<SmoothLifeRule>(
                birthMin, birthMax, surviveMin, surviveMax, alphaN, alphaM
            ),
            std::make_unique<EulerIntegrator>(dt),
            std::make_unique<ClampConstraint>(0.0, 1.0)
        )
    {}
};
