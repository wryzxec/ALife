#pragma once

#include <cstddef>
#include <memory>
#include <vector>

#include "alife/observation.hpp"
#include "core/fft_convolution.hpp"
#include "core/state.hpp"
#include "systems/lenia/interaction.hpp"

class ChannelConvObservation : public Observation {
public:
    ChannelConvObservation(
            std::vector<Interaction> interactions, 
            size_t rows, size_t cols
    ) {
        _interactions.reserve(interactions.size());

        for(const Interaction& interaction : interactions) {
            _interactions.push_back(
                    std::make_unique<RuntimeInteraction>(interaction, rows, cols)
            );
        }
    }

    Field observe(const State& state) override {
        Field potentials(state.rows(), state.cols(), _interactions.size());

        for(size_t i = 0; i < _interactions.size(); ++i) {
            const RuntimeInteraction& interaction = *_interactions[i];

            Field potential = interaction.convolution.convolve(
                    state, interaction.sourceChannel
            );

            copyPotentialToChannel(potentials, potential, i);
        }

        return potentials;
    }

private:
    struct RuntimeInteraction {
        size_t sourceChannel = 0;
        FFTChannelConv convolution;

        RuntimeInteraction(
            const Interaction& interaction,
            size_t rows, size_t cols
        )
            : sourceChannel{interaction.sourceChannel},
              convolution{interaction.kernel, rows, cols}
        {}
    };

    std::vector<std::unique_ptr<RuntimeInteraction>> _interactions;
    
    static void copyPotentialToChannel(Field& potentials, const Field& potential,
            size_t interactionIndex) {
        for(size_t r = 0; r < potentials.rows(); ++r) {
            for(size_t c = 0; c < potentials.cols(); ++c) {
                potentials(r, c, interactionIndex) = potential(r, c);
            }
        }
    }
};
