#pragma once

#include <cstddef>

#include "alife/observation.hpp"

/*
 * SmoothLife observation
 * 
 * Two-channel field:
 * channel 0 = m = inner disk density
 * channel 1 = n = outer disk density
 *
 * inner disk has radius r
 * outer disk covers r < distance <= 3r
 */

class SmoothObservation: public Observation {
public:
    SmoothObservation(size_t radius)
        : _radius{radius}
    {}

    Field observe(const State& state) override {
        Field observed(state.rows(), state.cols(), 2);

        for(size_t r = 0; r < state.rows(); ++r) {
            for(size_t c = 0; c < state.cols(); ++c) {
                const Densities densities = computeDensities(state, r, c);
                observed(r, c, 0) = densities.m;
                observed(r, c, 1) = densities.n;
            }
        }
        
        return observed;
    }

private:
    struct Densities {
        double m;
        double n;
    };

    size_t _radius;

    static size_t wrap(int index, size_t boundary) {
        int wrapped = index % static_cast<int>(boundary);
        if(wrapped < 0) wrapped += static_cast<int>(boundary);
        return static_cast<size_t>(wrapped);
    }

    Densities computeDensities(const State& state, size_t row, size_t col) const {
        double innerSum = 0.0;
        double outerSum = 0.0;

        int innerCount = 0;
        int outerCount = 0;

        const int innerRadius = static_cast<int>(_radius);
        const int outerRadius = static_cast<int>(3 * _radius);

        const int innerRadiusSq = innerRadius * innerRadius;
        const int outerRadiusSq = outerRadius * outerRadius;

        for(int dr = -outerRadius; dr <= outerRadius; ++dr) {
            for(int dc = -outerRadius; dc <= outerRadius; ++dc) {
                const int distSq = dr * dr + dc * dc;

                const size_t wrappedRow = wrap(static_cast<int>(row) + dr, state.rows());
                const size_t wrappedCol = wrap(static_cast<int>(col) + dc, state.cols());

                const double val = state(wrappedRow, wrappedCol);

                if(distSq <= innerRadiusSq) {
                    innerSum += val;
                    ++innerCount;
                } else if(distSq <= outerRadiusSq) {
                    outerSum += val;
                    ++outerCount;
                }
            }
        }

        const double m = innerCount > 0 ? innerSum / static_cast<double>(innerCount) : 0.0;
        const double n = outerCount > 0 ? outerSum / static_cast<double>(outerCount) : 0.0;

        return Densities{m, n};
    }
};
