#pragma once

#include "alife/updaterule.hpp"

/*
 *  survival range [surviveMin, surviveMax]: range in which alive cells can continue living.
 *  birth range [birthMin, birthMax]: range in which dead cells can become alive.
 */

class LargerThanLifeRule : public UpdateRule {
public:
    LargerThanLifeRule(int birthMin, int birthMax, int surviveMin, int surviveMax)
        : _birthMin{birthMin}, _birthMax{birthMax}, _surviveMin{surviveMin}, _surviveMax{surviveMax}
    {}

    Field apply(const State& state, const Field& nbrCount) override {
        Field next(state.rows(), state.cols(), state.channels());

        for(size_t r = 0; r < state.rows(); ++r) {
            for(size_t c = 0; c < state.cols(); ++c) {
                const bool alive = state(r, c) > 0.5;
                const int count = static_cast<int>(nbrCount(r, c));
                
                if(!alive && inRange(count, _birthMin, _birthMax)) {
                    next(r, c) = 1.0;
                } else if(alive && inRange(count, _surviveMin, _surviveMax)) {
                    next(r, c) = 1.0;
                } else {
                    next(r, c) = 0.0;
                }
            }
        }

        return next;
    }
private:
    static bool inRange(int val, int min, int max) {
        return val >= min && val <= max;
    }

    int _birthMin;
    int _birthMax;
    int _surviveMin;
    int _surviveMax;
};
