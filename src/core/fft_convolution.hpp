#pragma once

#include <cstddef>
#include <stdexcept>
#include <vector>

#include <kiss_fftnd.h>

#include "core/fft2d.hpp"
#include "core/kernel.hpp"
#include "core/state.hpp"

class FFTChannelConv {
public:
    FFTChannelConv(
        const Kernel& kernel,
        size_t rows,
        size_t cols
    )
        : _fft{rows, cols},
          _kernelFFT{precomputeKernelFFT(kernel)}
    {}

    Field convolve(const State& state, size_t srcChannel) const {

        if(state.rows() != _fft.rows() || state.cols() != _fft.cols()) {
            throw std::invalid_argument("State dimensions do not match FFTChannelConv dimensions.");
        }

        if(srcChannel >= state.channels()) {
            throw std::invalid_argument("Source channel is out of range.");
        }

        const std::vector<kiss_fft_cpx> stateSpatial = stateToComplexBuffer(state, srcChannel);
        const std::vector<kiss_fft_cpx> stateFFT = _fft.forward(stateSpatial);
        const std::vector<kiss_fft_cpx> potentialFFT = multiplySpectra(stateFFT, _kernelFFT);
        const std::vector<kiss_fft_cpx> potentialSpatial = _fft.inverse(potentialFFT);

        return complexBufferToField(potentialSpatial);
    }

private:
    static kiss_fft_cpx complexZero() {
        return kiss_fft_cpx{ static_cast<kiss_fft_scalar>(0.0), static_cast<kiss_fft_scalar>(0.0) };
    }

    static kiss_fft_cpx complexFromReal(double value) {
        return kiss_fft_cpx{ static_cast<kiss_fft_scalar>(value), static_cast<kiss_fft_scalar>(0.0) };
    }

    static kiss_fft_cpx complexMultiply(kiss_fft_cpx a, kiss_fft_cpx b) {
        return kiss_fft_cpx{
            static_cast<kiss_fft_scalar>(a.r * b.r - a.i * b.i),
            static_cast<kiss_fft_scalar>(a.r * b.i + a.i * b.r)
        };
    }

    static size_t wrap(int index, size_t boundary) {
        const int b = static_cast<int>(boundary);
        int wrapped = index % b;
        if (wrapped < 0) wrapped += b;

        return static_cast<size_t>(wrapped);
    }

    std::vector<kiss_fft_cpx> precomputeKernelFFT(const Kernel& kernel) const {
        std::vector<kiss_fft_cpx> kernelSpatial( _fft.size(), complexZero());

        const int centreRow = static_cast<int>(kernel.rows() / 2);
        const int centreCol = static_cast<int>(kernel.cols() / 2);

        for (size_t kr = 0; kr < kernel.rows(); ++kr) {
            for (size_t kc = 0; kc < kernel.cols(); ++kc) {
                const int dr = static_cast<int>(kr) - centreRow;
                const int dc = static_cast<int>(kc) - centreCol;

                const size_t row = wrap(dr, _fft.rows());
                const size_t col = wrap(dc, _fft.cols());

                kernelSpatial[_fft.index(row, col)] = complexFromReal(kernel(kr, kc));
            }
        }

        return _fft.forward(kernelSpatial);
    }

    std::vector<kiss_fft_cpx> stateToComplexBuffer(const State& state, size_t srcChannel) const {
        std::vector<kiss_fft_cpx> buffer(_fft.size());

        for (size_t row = 0; row < state.rows(); ++row) {
            for (size_t col = 0; col < state.cols(); ++col) {
                buffer[_fft.index(row, col)] =
                    complexFromReal(state(row, col, srcChannel));
            }
        }

        return buffer;
    }

    std::vector<kiss_fft_cpx> multiplySpectra(const std::vector<kiss_fft_cpx>& a, 
            const std::vector<kiss_fft_cpx>& b) const {

        std::vector<kiss_fft_cpx> result(_fft.size());

        for (size_t i = 0; i < _fft.size(); ++i) {
            result[i] = complexMultiply(a[i], b[i]);
        }

        return result;
    }

    Field complexBufferToField(const std::vector<kiss_fft_cpx>& buffer) const {
        Field field(_fft.rows(), _fft.cols(), 1);

        const double scale = 1.0 / static_cast<double>(_fft.size());

        for (size_t row = 0; row < _fft.rows(); ++row) {
            for (size_t col = 0; col < _fft.cols(); ++col) {
                field(row, col) = static_cast<double>(buffer[_fft.index(row, col)].r) * scale;
            }
        }

        return field;
    }

    FFT2D _fft;
    std::vector<kiss_fft_cpx> _kernelFFT;
};
