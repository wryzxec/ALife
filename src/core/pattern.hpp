#pragma once

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

#include "core/grid.hpp"

class Pattern {
public:
    Pattern(size_t rows, size_t cols, size_t channels = 1)
        : _grid(rows, cols, channels)
    {}

    Pattern(size_t rows, size_t cols, size_t channels,
            std::initializer_list<double> values)
        : _grid(rows, cols, channels)
    {
        if(values.size() != rows * cols * channels) {
            throw std::invalid_argument("Pattern initialiser list does not match rows * cols * channels.");
        }

        auto it = values.begin();
        for(size_t r = 0; r < rows; ++r) {
            for(size_t c = 0; c < cols; ++c) {
                for(size_t ch = 0; ch < channels; ++ch) {
                    _grid(r, c, ch) = *it;
                    ++it;
                }
            }
        }
    }

    Pattern(size_t rows, size_t cols, std::initializer_list<double> values)
        : Pattern(rows, cols, 1, values)
    {}

    double& operator()(size_t r, size_t c, size_t channel = 0) { return _grid(r, c, channel); }
    const double& operator()(size_t r, size_t c, size_t channel = 0) const { return _grid(r, c, channel); }

    size_t rows() const { return _grid.rows(); }
    size_t cols() const { return _grid.cols(); }
    size_t channels() const { return _grid.channels(); }
    
private:
    Grid<double> _grid;
};
