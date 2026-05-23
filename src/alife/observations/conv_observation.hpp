#pragma once

#include <cstddef>
#include <utility>

#include "alife/observation.hpp"
#include "core/kernel.hpp"

class ConvObservation : public Observation {
public:
    ConvObservation(Kernel kernel) : _kernel{std::move(kernel)} {}

    Field observe(const State& state) override {
        Field observed(state.rows(), state.cols(), state.channels());

        for(size_t r = 0; r < state.rows(); ++r) {
            for(size_t c = 0; c < state.cols(); ++c) {
                observed(r, c) = convolveAt(state, r, c);
            }
        }

        return observed;
    }

private:
    static size_t wrap(int index, size_t boundary) {
        int wrapped = index % static_cast<int>(boundary);
        if(wrapped < 0) wrapped += static_cast<int>(boundary);
        return static_cast<size_t>(wrapped);
    }

    double convolveAt(const State& state, size_t r, size_t c) const {
        double sum = 0.0;

        const int centreRowKern = static_cast<int>(_kernel.rows() / 2);
        const int centreColKern = static_cast<int>(_kernel.cols() / 2);

        for(size_t kr = 0; kr < _kernel.rows(); ++kr) {
            for(size_t kc = 0; kc < _kernel.cols(); ++kc) {
                // get row/column offset from kernel center.
                const int dr = static_cast<int>(kr) - centreRowKern;
                const int dc = static_cast<int>(kc) - centreColKern;

                // use dr, dc to calculate relative offset from r, c in state.
                const size_t stateRow = wrap(static_cast<int>(r) + dr, state.rows());
                const size_t stateCol = wrap(static_cast<int>(c) + dc, state.cols());

                sum += state(stateRow, stateCol) * _kernel(kr, kc);                               
            }
        }

        return sum;
    }

    Kernel _kernel;
};
