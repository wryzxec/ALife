#pragma once

#include "alife/integrator.hpp"

class EulerIntegrator : public Integrator {
public:
    EulerIntegrator(double dt) : _dt{dt} {}

    void integrate(State& state, const Field& update) override {
        for(size_t r = 0; r < state.rows(); ++r) {
            for(size_t c = 0; c < state.cols(); ++c) {
                state(r, c) += _dt * update(r, c);
            }
        }
    }

private:
    double _dt;
};
