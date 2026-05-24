#pragma once

#include <memory>
#include <utility>

#include "alife/alife.hpp"
#include "alife/constraints/clamp_constraint.hpp"
#include "alife/observations/conv_observation.hpp"
#include "alife/observations/fft_conv_observation.hpp"
#include "alife/integrators/euler_integrator.hpp"
#include "alife/rules/lenia_rule.hpp"
#include "core/kernel.hpp"

class Lenia : public ALife {
public:
    Lenia(
            State initialState, Kernel kernel,
            double mu, double sigma, double dt
    )
        : ALife(
            std::move(initialState),
            std::make_unique<FFTConvObservation>(std::move(kernel), initialState.rows(), initialState.cols()),
            std::make_unique<LeniaRule>(mu, sigma),
            std::make_unique<EulerIntegrator>(dt),
            std::make_unique<ClampConstraint>(0.0, 1.0)
        )
    {}
};
