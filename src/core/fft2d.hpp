#pragma once

#include <cstddef>
#include <stdexcept>
#include <vector>

#include <kiss_fftnd.h>

class FFT2D {
public:
    FFT2D(size_t rows, size_t cols)
        : _rows{rows}, _cols{cols}, _size{rows * cols},
          _dims{static_cast<int>(rows), static_cast<int>(cols)}
    {
        if(rows == 0 || cols == 0) throw std::invalid_argument("FFT2D arguments must be nonzero.");

        _forward = kiss_fftnd_alloc(_dims.data(), 2, 0, nullptr, nullptr);
        _inverse = kiss_fftnd_alloc(_dims.data(), 2, 1, nullptr, nullptr);

        if(_forward == nullptr || _inverse == nullptr) {
            throw std::runtime_error("Failed to allocate kissFFT configs.");
        }
    }

    ~FFT2D() {
        if(_forward) kiss_fft_free(_forward);
        if(_inverse) kiss_fft_free(_inverse);
    }

    FFT2D(const FFT2D&) = delete;
    FFT2D& operator=(const FFT2D&) = delete;

    std::vector<kiss_fft_cpx> forward(const std::vector<kiss_fft_cpx>& spatial) const {
        std::vector<kiss_fft_cpx> spectrum(_size);
        kiss_fftnd(_forward, spatial.data(), spectrum.data());
        return spectrum;
    }

    std::vector<kiss_fft_cpx> inverse(const std::vector<kiss_fft_cpx>& spectrum) const {
        std::vector<kiss_fft_cpx> spatial(_size);
        kiss_fftnd(_inverse, spectrum.data(), spatial.data());
        return spatial;
    }

    size_t rows() const { return _rows; }
    size_t cols() const { return _cols; }
    size_t size() const { return _size; }

    size_t index(size_t r, size_t c) const { return r * _cols + c; }

private:
    size_t _rows;
    size_t _cols;
    size_t _size;

    std::vector<int> _dims;

    kiss_fftnd_cfg _forward = nullptr;
    kiss_fftnd_cfg _inverse = nullptr;
};
