#pragma once

#include "core/state.hpp"

class Constraint {
public:
    virtual void apply(State& state) = 0;
    virtual ~Constraint() = default;
};
