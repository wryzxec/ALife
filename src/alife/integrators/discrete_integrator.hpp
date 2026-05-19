#include "alife/integrator.hpp"

class DiscreteIntegrator : public Integrator {
    void integrate(State& state, const Field& update) {
        for(size_t r = 0; r < state.rows(); ++r) {
            for(size_t c = 0; c < state.cols(); ++c) {
                state(r, c) = update(r, c);
            }
        }
    }
};
