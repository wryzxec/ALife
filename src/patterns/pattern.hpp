#pragma once

#include <cmath>
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

    void scale(double factor) {

        if(factor <= 0.0) throw std::invalid_argument("Scale factor must be positive.");
        if(factor == 1.0) return;

        const size_t oldRows = rows();
        const size_t oldCols = cols();
        const size_t oldChannels = channels();

        const size_t newRows =
            static_cast<size_t>(std::round(static_cast<double>(oldRows) * factor));
        const size_t newCols =
            static_cast<size_t>(std::round(static_cast<double>(oldCols) * factor));

        Grid<double> scaled(newRows, newCols, oldChannels);

        for(size_t r = 0; r < newRows; ++r) {
            for(size_t c = 0; c < newCols; ++c) {
                const size_t srcR = std::min(
                    static_cast<size_t>(std::floor(static_cast<double>(r) / factor)),
                    oldRows - 1
                );

                const size_t srcC = std::min(
                    static_cast<size_t>(std::floor(static_cast<double>(c) / factor)),
                    oldCols - 1
                );

                for(size_t ch = 0; ch < oldChannels; ++ch) {
                    scaled(r, c, ch) = _grid(srcR, srcC, ch);
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
