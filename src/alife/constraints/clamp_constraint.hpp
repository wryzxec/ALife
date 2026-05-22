#pragma once

#include <algorithm>

#include "alife/constraint.hpp"

class ClampConstraint : public Constraint {
public:
    ClampConstraint(double minVal, double maxVal)
        : _minVal(minVal), _maxVal(maxVal)
    {}

    void apply(State& state) override {
        for(size_t r = 0; r < state.rows(); ++r) {
            for(size_t c = 0; c < state.cols(); ++c) {
                state(r, c) = std::clamp(state(r, c), _minVal, _maxVal);
            }
        }
    }

private:
    double _minVal;
    double _maxVal;
};
