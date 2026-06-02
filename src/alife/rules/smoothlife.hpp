#pragma once

#include <cmath>

#include "alife/updaterule.hpp"

class SmoothLifeRule : public UpdateRule {
public:
    SmoothLifeRule(
            double birthMin, double birthMax, 
            double surviveMin, double surviveMax,
            double alphaN, double alphaM
    ) : _birthMin{birthMin}, _birthMax{birthMax},
        _surviveMin{surviveMin}, _surviveMax{surviveMax},
        _alphaN{alphaN}, _alphaM{alphaM}
    {}

    Field apply(const State& state, const Field& observed) override {
        Field update(state.rows(), state.cols(), state.channels());

        for(size_t r = 0; r < state.rows(); ++r) {
            for(size_t c = 0; c < state.cols(); ++c) {
                const double m = observed(r, c, 0);
                const double n = observed(r, c, 1);

                const double target = transition(n, m);
                update(r, c) = target - state(r, c);
            }
        }

        return update;
    }
private:

    // smooth version of x >= a
    static double sigma(double x, double a, double alpha) {
        return 1.0 / (1.0 + std::exp(-4.0 * (x - a) / alpha));
    }

    // smooth version of a <= x <= b
    static double sigmaN(double x, double a, double b, double alpha) {
        return sigma(x, a, alpha) * (1 - sigma(x, b, alpha));
    }

    static double sigmaM(double x, double y, double m, double alpha) {
        const double val = sigma(m, 0.5, alpha); // m >= 0.5

        return (x * (1.0 - val)) + (y * val); // if val = 1 then return y, otherwise if 0 then return x
    }

    // choose which boundary we are concerned with, based on our inner density m.
    // Then evaluate whether our outer density n lies within that boundary.
    double transition(double n, double m) const {
        const double lower = sigmaM(_birthMin, _surviveMin, m, _alphaM);
        const double upper = sigmaM(_birthMax, _surviveMax, m, _alphaM);

        return sigmaN(n, lower, upper, _alphaN);
    }

    double _birthMin;
    double _birthMax;
    double _surviveMin;
    double _surviveMax;
    double _alphaN;
    double _alphaM;
};
