#pragma once

#include <cstddef>
#include <random>

#include "grid.hpp"

/*
 * State is the current world.
 * A field is spatial information (convolution, neighbour count, etc.) which is
 * used to update the state.
 */

using Scalar = double;
using Field = Grid<Scalar>;

class State {
public:
    State(size_t rows, size_t cols, size_t channels = 1)
        : _grid(rows, cols, channels)
    {}

    Scalar& operator()(size_t row, size_t col, size_t channel = 0) {
        return _grid(row, col, channel);
    }

    const Scalar& operator()(size_t row, size_t col, size_t channel = 0) const {
        return _grid(row, col, channel);
    }

    size_t rows() const { return _grid.rows(); }
    size_t cols() const { return _grid.cols(); }
    size_t channels() const { return _grid.channels(); }

    void randomiseBinary(double probability) {
        std::mt19937 rng(std::random_device{}());
        std::bernoulli_distribution dist{probability};

        for(size_t r = 0; r < rows(); ++r) {
            for(size_t c = 0; c < cols(); ++c) {
                _grid(r, c) = dist(rng) ? 1.0 : 0.0;
            }
        }
    }

    void randomiseContinuous(double minValue = 0.0, double maxValue = 1.0) {
        std::mt19937 rng(std::random_device{}());
        std::uniform_real_distribution dist{minValue, maxValue};

        for(size_t r = 0; r < rows(); ++r) {
            for(size_t c = 0; c < cols(); ++c) {
                _grid(r, c) = dist(rng);
            }
        }
    }

    void randomiseContinuousDisc(size_t radius, double minValue, double maxValue) {
        std::mt19937 rng(std::random_device{}());
        std::uniform_real_distribution dist{minValue, maxValue};

        const size_t centreRow = rows() / 2;
        const size_t centreCol = cols() / 2;

        const double radiusSq = static_cast<double>(radius * radius);

        for(size_t r = 0; r < rows(); ++r) {
            for(size_t c = 0; c < cols(); ++c) {
                const double dr = static_cast<double>(r) - static_cast<double>(centreRow);
                const double dc = static_cast<double>(c) - static_cast<double>(centreCol);

                const double distSq = dr * dr + dc * dc;

                if(distSq <= radiusSq) {
                    _grid(r, c) = dist(rng);
                }
            }
        }
    }

private:
    Grid<Scalar> _grid;
};
