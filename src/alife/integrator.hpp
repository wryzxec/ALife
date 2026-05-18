#pragma once

#include "core/state.hpp"

class Integrator {
public:
    virtual void integrate(State& state, const Field& update) = 0;
    virtual ~Integrator() = default;
};
