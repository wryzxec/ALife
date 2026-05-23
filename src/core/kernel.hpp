#pragma once

#include <cmath>
#include <cstddef>
#include <numeric>
#include <stdexcept>
#include <vector>

#include "core/grid.hpp"

class Kernel {
public:
    Kernel(size_t rows, size_t cols) : _weights(rows, cols, 1) {}

    /*
     * Specialised kernel constructors:
     * - uniformSquare: square kernel where each cell has equal weight.
     * - gaussian: radially symmetric gaussian kernel. Largest at centre, and decreases with distance.
     * - multiRing: multiple smooth concentric rings. Beta vector controls strength of each ring.
     */

    static Kernel uniformSquare(size_t sideLength) {
        Kernel kernel(sideLength, sideLength);

        for(size_t r = 0; r < kernel.rows(); ++r) {
            for(size_t c = 0; c < kernel.cols(); ++c) {
                kernel(r, c) = 1.0;
            }
        }

        kernel.normalise();
        return kernel;
    }

    static Kernel gaussian(size_t radius, double sigma) {
        if(sigma <= 0.0) throw std::invalid_argument("Gaussian sigma must be greater than zero.");

        const size_t size = 2 * radius + 1;
        Kernel kernel(size, size);

        const double centre = static_cast<double>(radius);

        for(size_t r = 0; r < size; ++r) {
            for(size_t c = 0; c < size; ++c) {
                const double dr = static_cast<double>(r) - centre;
                const double dc = static_cast<double>(c) - centre;

                const double distSq = dr * dr + dc * dc;
                
                kernel(r, c) = std::exp(-distSq / (2.0 * sigma * sigma));
            }
        }

        kernel.normalise();
        return kernel;
    }

    static Kernel multiRing(size_t radius, const std::vector<double>& beta, double alpha = 4.0) {
        if(radius == 0) throw std::invalid_argument("Kernel radius must be greater than zero.");
        if(beta.empty()) throw std::invalid_argument("Beta must contain at least one value.");

        const size_t size = 2 * radius + 1;
        Kernel kernel(size, size);

        const double centre = static_cast<double>(radius);
        const size_t bands = beta.size();

        for(size_t r = 0; r < size; ++r) {
            for(size_t c = 0; c < size; ++c) {
                const double dr = static_cast<double>(r) - centre;
                const double dc = static_cast<double>(c) - centre;
                
                const double dist = std::sqrt(dr * dr + dc * dc);
                const double distNorm = dist / static_cast<double>(radius);

                if(distNorm >= 1.0) {
                    kernel(r, c) = 0.0;
                    continue;
                }

                const double Br = static_cast<double>(bands) * distNorm;
                const size_t band = static_cast<size_t>(std::floor(Br));
                
                const double localDist = Br - std::floor(Br);
                kernel(r, c) = beta[band] * kernelCore(localDist, alpha);
            }
        }

        kernel.normalise();
        return kernel;
    }

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
    static double kernelCore(double r, double alpha) {
        if(r <= 0.0 || r >= 1.0) {
            return 0.0;
        }

        return std::exp(alpha - alpha / (4.0 * r * (1.0 - r)));
    }

    Grid<double> _weights;
};
