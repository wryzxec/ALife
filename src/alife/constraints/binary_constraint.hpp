#pragma once

#include "alife/constraint.hpp"

class BinaryConstraint : public Constraint {
    void apply(State& state) override {
        for(size_t r = 0; r < state.rows(); ++r) {
            for(size_t c = 0; c < state.cols(); ++c) {
                state(r, c) = state(r, c) > 0.5 ? 1.0 : 0.0;
            }
        }
    }
};
