#pragma once

#include <cmath>
#include <cstddef>
#include <utility>
#include <vector>

#include "alife/updaterule.hpp"
#include "systems/lenia/interaction.hpp"

class LeniaRule : public UpdateRule {
public:
    LeniaRule(std::vector<Interaction> interactions)
        : _interactions{std::move(interactions)}
    {}

    Field apply(const State& state, const Field& potentials) override {
        Field delta(state.rows(), state.cols(), state.channels());

        for(size_t i = 0; i < _interactions.size(); ++i) {
            const Interaction& interaction = _interactions[i];
            for(size_t r = 0; r < state.rows(); ++r) {
                for(size_t c = 0; c < state.cols(); ++c) {
                    const double u = potentials(r, c, i);
                    const size_t ch = interaction.targetChannel;
                    const double w =interaction.weight;

                    if(interaction.updateMode == UpdateMode::Classic) {
                        delta(r, c, ch) += w * growth(
                            u, interaction.growthMu, interaction.growthSigma
                        );
                    } else {
                        delta(r, c, ch) += w * bell(
                            u, interaction.growthMu, interaction.growthSigma
                        ) - state(r, c, ch);
                    }
                }
            }
        }

        return delta;
    }

private:
    static double bell(double u, double mu, double sigma) {
        const double diff = u - mu;
        const double exp = -(diff * diff) / (2.0 * sigma * sigma);
        return std::exp(exp);
    }

    static double growth(double u, double mu, double sigma) {
        return 2.0 * bell(u, mu, sigma) - 1.0;
    }

    std::vector<Interaction> _interactions;
};
