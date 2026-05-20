#pragma once

#include <cstddef>

#include "alife/observation.hpp"

class NbrObservation : public Observation {
public:
    NbrObservation(std::size_t radius = 1)
        : _radius{radius}
    {}

    Field observe(const State& state) override {
        Field nbrCounts(state.rows(), state.cols(), state.channels());

        for(size_t r = 0; r < state.rows(); ++r) {
            for(size_t c = 0; c < state.cols(); ++c) {
                nbrCounts(r, c) = countLiveNeighbours(state, r, c);
            }
        }

        return nbrCounts;
    }
private:
    static size_t wrap(int index, size_t boundary) {
        int wrapped = index % static_cast<int>(boundary);
        if(wrapped < 0) wrapped += static_cast<int>(boundary);
        return static_cast<size_t>(wrapped);
    }

    int countLiveNeighbours(const State& state, size_t row, size_t col) const {
        int count = 0;
        const int radius = static_cast<int>(_radius);

        for(int dr = -radius; dr <= radius; ++dr) {
            for(int dc = -radius; dc <= radius; ++dc) {
                if(dr == 0 && dc == 0) continue; // dont count centre cell
                size_t wrappedRow = wrap(static_cast<int>(row) + dr, state.rows());
                size_t wrappedCol = wrap(static_cast<int>(col) + dc, state.cols());
                if(state(wrappedRow, wrappedCol) > 0.5) count++;
            }
        }

        return count;
    }

    /*
     * radius of square neighbourhood we observe. e.g. "moore radius" used in Conway's
     * Game of Life is the 3x3 neighbourhood around a given cell, excluding the cell itself:
     *
     * [o o o]
     * [o x o]          radius = 1
     * [o o o]
     */
    size_t _radius;
};
