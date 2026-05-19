#pragma once

#include <cstddef>
#include <vector>

/*
 *  2D grid with optional channel dimension.
 *
 *  Grid stores values in 1D vector.
 *
 *  Each cell may contain one or more channel values.
 *
 *  Example:
 *      grid(row, col, 0) is first channel at cell,
 *      grid(row, col, 1) is second channel at cell.
 */

template<typename T>
class Grid {
public:
    Grid(size_t rows, size_t cols, size_t channels = 1) : 
        _rows{rows}, _cols{cols}, 
        _channels{channels}, 
        _data(rows*cols*channels)
    {}
    
    T& operator()(size_t row, size_t col, size_t channel = 0) {
        return _data[index(row, col, channel)];
    }

    const T& operator()(size_t row, size_t col, size_t channel = 0) const {
        return _data[index(row, col, channel)];
    }

    size_t rows() const { return _rows; }
    size_t cols() const { return _cols; }
    size_t channels() const { return _channels; }
    size_t size() const { return _data.size(); }
private:
    size_t index(size_t row, size_t col, size_t channel) const {
        return (row * _cols + col) * _channels + channel;
    }

    size_t _rows;
    size_t _cols;
    size_t _channels;
    std::vector<T> _data;
};
