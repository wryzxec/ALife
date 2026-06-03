#pragma once

#include "alife/integrator.hpp"
#include <cstddef>

class EulerIntegrator : public Integrator {
public:
    EulerIntegrator(double dt) : _dt{dt} {}

    void integrate(State& state, const Field& update) override {
        for(size_t r = 0; r < state.rows(); ++r) {
            for(size_t c = 0; c < state.cols(); ++c) {
                for(size_t ch = 0; ch < state.channels(); ++ch) {
                    state(r, c, ch) += _dt * update(r, c, ch);
                }
            }
        }
    }

private:
    double _dt;
};
