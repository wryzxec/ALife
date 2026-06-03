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
        Field growth(state.rows(), state.cols(), state.channels());

        for(size_t i = 0; i < _interactions.size(); ++i) {
            const Interaction& interaction = _interactions[i];
            accumInteractionGrowth(growth, potentials, i, interaction);
        }

        return growth;
    }
private:
    static double growthFunction(double u, double mu, double sigma) {
        const double diff = u - mu;
        const double exp = -(diff * diff) / (2.0 * sigma * sigma);

        return 2.0 * std::exp(exp) - 1.0;
    }

    static void accumInteractionGrowth(Field& growth, const Field& potentials,
            size_t interactionIndex, const Interaction& interaction) {
        for(size_t r = 0; r < growth.rows(); ++r) {
            for(size_t c = 0; c < growth.cols(); ++c) {
                const double u = potentials(r, c, interactionIndex);
                const double g = growthFunction(
                        u, interaction.growthMu, interaction.growthSigma
                );

                growth(r, c, interaction.targetChannel) += 
                    interaction.weight * g;
            }
        }
    }

    std::vector<Interaction> _interactions;
};
