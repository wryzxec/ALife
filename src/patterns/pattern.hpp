#pragma once

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>

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
        assignValues(values.begin(), values.size());
    }

    Pattern(size_t rows, size_t cols, std::initializer_list<double> values)
        : Pattern(rows, cols, 1, values)
    {}

    Pattern(size_t rows, size_t cols, size_t channels, const std::vector<double>& values)
        : _grid(rows, cols, channels)
    {
        assignValues(values.begin(), values.size());
    }

    void scale(size_t factor) {
        if(factor <= 1) return;

        const size_t oldRows = rows();
        const size_t oldCols = cols();

        Grid<double> scaled(oldRows * factor, oldCols * factor, channels());

        for(size_t r = 0; r < oldRows; ++r) {
            for(size_t c = 0; c < oldCols; ++c) {
                for(size_t dr = 0; dr < factor; ++dr) {
                    for(size_t dc = 0; dc < factor; ++dc) {
                        for(size_t ch = 0; ch < channels(); ++ch) {
                            scaled(
                                r * factor + dr,
                                c * factor + dc,
                                ch
                            ) = _grid(r, c, ch);
                        }
                    }
                }
            }
        }

        _grid = std::move(scaled);
    }

    double& operator()(size_t r, size_t c, size_t channel = 0) { return _grid(r, c, channel); }
    const double& operator()(size_t r, size_t c, size_t channel = 0) const { return _grid(r, c, channel); }

    size_t rows() const { return _grid.rows(); }
    size_t cols() const { return _grid.cols(); }
    size_t channels() const { return _grid.channels(); }
    
private:
    template<typename Iterator>
    void assignValues(Iterator begin, size_t count) {
        if(count != rows() * cols() * channels()) {
            throw std::invalid_argument("Number of pattern values does not match pattern dimensions.");
        }

        auto it = begin;
        for(size_t r = 0; r < rows(); ++r) {
            for(size_t c = 0; c < cols(); ++c) {
                for(size_t ch = 0; ch < channels(); ++ch) {
                    _grid(r, c, ch) = *it;
                    ++it;
                }
            }
        }
    }

    Grid<double> _grid;
};
