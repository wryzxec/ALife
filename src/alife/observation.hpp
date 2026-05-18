#pragma once

#include "core/state.hpp"

class Observation {
public:
    virtual Field observe(const State& state) = 0;
    virtual ~Observation() = default;
};
