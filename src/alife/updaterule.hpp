#pragma once

#include "core/state.hpp"

class UpdateRule {
public:
    virtual Field apply(const State& state, const Field& observed) = 0;
    virtual ~UpdateRule() = default;
};
