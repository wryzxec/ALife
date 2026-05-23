#pragma once

#include <cstddef>
#include <numeric>

#include "core/grid.hpp"

class Kernel {
public:
    Kernel(size_t rows, size_t cols) : _weights(rows, cols, 1) {}

    double& operator()(size_t r, size_t c) { return _weights(r, c); }
    const double& operator()(size_t r, size_t c) const { return _weights(r, c); }

    size_t rows() const { return _weights.rows(); }
    size_t cols() const { return _weights.cols(); }

    void normalise() {
        const std::vector<double>& data = _weights.data();
        double sum = std::accumulate(
                data.begin(),
                data.end(),
                0.0
        );

        if(sum == 0) return;

        for(size_t r = 0; r < _weights.rows(); ++r) {
            for(size_t c = 0; c < _weights.cols(); ++c) {
                _weights(r, c) /= sum;
            }
        }
    }
private:
    Grid<double> _weights;
};
