#pragma once

#include <cmath>

#include "alife/updaterule.hpp"

class LeniaRule : public UpdateRule {
public:
    LeniaRule(double mu, double sigma)
        : _mu{mu}, _sigma{sigma}
    {}

    Field apply(const State& state, const Field& potential) override {
        Field growth(state.rows(), state.cols(), state.channels());

        for(size_t r = 0; r < state.rows(); ++r) {
            for(size_t c = 0; c < state.cols(); ++c) {
                const double u = potential(r, c);
                growth(r, c) = growthFunction(u);
            }
        }

        return growth;
    }
private:
    double growthFunction(double u) const {
        const double diff = u - _mu;
        const double exp = -(diff * diff) / (2.0 * _sigma * _sigma);

        return 2.0 * std::exp(exp) - 1.0;
    }

    double _mu;
    double _sigma;
};
