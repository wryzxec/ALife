#pragma once

#include <algorithm>
#include <cstddef>

#include "alife/constraint.hpp"

class ClampConstraint : public Constraint {
public:
    ClampConstraint(double minVal, double maxVal)
        : _minVal(minVal), _maxVal(maxVal)
    {}

    void apply(State& state) override {
        for(size_t r = 0; r < state.rows(); ++r) {
            for(size_t c = 0; c < state.cols(); ++c) {
                for(size_t ch = 0; ch < state.channels(); ++ch) {
                    state(r, c, ch) = std::clamp(state(r, c, ch), _minVal, _maxVal);
                }
            }
        }
    }

private:
    double _minVal;
    double _maxVal;
};
