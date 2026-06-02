#pragma once

#include <memory>
#include <utility>

#include "alife/alife.hpp"
#include "alife/constraints/clamp.hpp"
#include "alife/observations/fft_convolution.hpp"
#include "alife/integrators/euler.hpp"
#include "alife/rules/lenia.hpp"
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
