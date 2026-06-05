#pragma once

#include <cmath>

#include "alife/constraint.hpp"

class SigmoidConstraint : public Constraint {
public:
    SigmoidConstraint() = default;

    void apply(State& state) override {
        for(size_t r = 0; r < state.rows(); ++r) {
            for(size_t c = 0; c < state.cols(); ++c) {
                for(size_t ch = 0; ch < state.channels(); ++ch) {
                    state(r, c, ch) = sigmoid(state(r, c, ch));
                }
            }
        }
    }
private:
    double sigmoid(double val) const {
        return 1.0 / (1.0 + std::exp(-4.0 * (val - 0.5)));
    }
};
