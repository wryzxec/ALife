#pragma once

#include <cstddef>
#include <utility>
#include <vector>

#include <kiss_fftnd.h>

#include "alife/observation.hpp"
#include "core/fft2d.hpp"
#include "core/kernel.hpp"

class FFTConvObservation : public Observation {
public:
    FFTConvObservation(
        Kernel kernel,
        std::size_t rows,
        std::size_t cols
    )
        : _kernel{std::move(kernel)},
          _fft{rows, cols},
          _kernelFFT{precomputeKernelFFT()}
    {}

    Field observe(const State& state) override {
        const std::vector<kiss_fft_cpx> stateSpatial = stateToComplexBuffer(state);
        const std::vector<kiss_fft_cpx> stateFFT = _fft.forward(stateSpatial);
        const std::vector<kiss_fft_cpx> potentialFFT = multiplySpectra(stateFFT, _kernelFFT);
        const std::vector<kiss_fft_cpx> potentialSpatial = _fft.inverse(potentialFFT);

        return complexBufferToField(potentialSpatial, state);
    }

private:
    Kernel _kernel;
    FFT2D _fft;
    std::vector<kiss_fft_cpx> _kernelFFT;

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

    static std::size_t wrap(int index, std::size_t boundary) {
        const int b = static_cast<int>(boundary);
        int wrapped = index % b;
        if (wrapped < 0) wrapped += b;

        return static_cast<std::size_t>(wrapped);
    }

    std::vector<kiss_fft_cpx> precomputeKernelFFT() const {
        std::vector<kiss_fft_cpx> kernelSpatial( _fft.size(), complexZero());

        const int centreRow = static_cast<int>(_kernel.rows() / 2);
        const int centreCol = static_cast<int>(_kernel.cols() / 2);

        for (std::size_t kr = 0; kr < _kernel.rows(); ++kr) {
            for (std::size_t kc = 0; kc < _kernel.cols(); ++kc) {
                const int dr = static_cast<int>(kr) - centreRow;
                const int dc = static_cast<int>(kc) - centreCol;

                const std::size_t row = wrap(dr, _fft.rows());
                const std::size_t col = wrap(dc, _fft.cols());

                kernelSpatial[_fft.index(row, col)] = complexFromReal(_kernel(kr, kc));
            }
        }

        return _fft.forward(kernelSpatial);
    }

    std::vector<kiss_fft_cpx> stateToComplexBuffer(const State& state) const {
        std::vector<kiss_fft_cpx> buffer(_fft.size());

        for (std::size_t row = 0; row < state.rows(); ++row) {
            for (std::size_t col = 0; col < state.cols(); ++col) {
                buffer[_fft.index(row, col)] =
                    complexFromReal(state(row, col));
            }
        }

        return buffer;
    }

    std::vector<kiss_fft_cpx> multiplySpectra(const std::vector<kiss_fft_cpx>& a, 
            const std::vector<kiss_fft_cpx>& b) const {

        std::vector<kiss_fft_cpx> result(_fft.size());

        for (std::size_t i = 0; i < _fft.size(); ++i) {
            result[i] = complexMultiply(a[i], b[i]);
        }

        return result;
    }

    Field complexBufferToField(const std::vector<kiss_fft_cpx>& buffer, const State& state) const {
        Field field(state.rows(), state.cols(), state.channels());

        const double scale = 1.0 / static_cast<double>(_fft.size());

        for (std::size_t row = 0; row < state.rows(); ++row) {
            for (std::size_t col = 0; col < state.cols(); ++col) {
                field(row, col) = static_cast<double>(buffer[_fft.index(row, col)].r) * scale;
            }
        }

        return field;
    }
};
