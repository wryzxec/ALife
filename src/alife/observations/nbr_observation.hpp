#include <cstddef>

#include "alife/observation.hpp"

class NbrObservation : public Observation {
public:
    Field observe(const State& state) override {
        Field nbrCounts(state.rows(), state.cols(), state.channels());

        for(size_t r = 0; r < state.rows(); ++r) {
            for(size_t c = 0; c < state.cols(); ++c) {
                nbrCounts(r, c) = countLiveNeighbours(state, r, c);
            }
        }

        return nbrCounts;
    }
private:
    static size_t wrap(int index, size_t boundary) {
        int wrapped = index % static_cast<int>(boundary);
        if(wrapped < 0) wrapped += static_cast<int>(boundary);
        return static_cast<size_t>(wrapped);
    }

    static int countLiveNeighbours(const State& state, size_t row, size_t col) {
        int count = 0;

        for(int dr = -1; dr <= 1; ++dr) {
            for(int dc = -1; dc <= 1; ++dc) {
                if(dr == 0 && dc == 0) continue; // dont count centre cell
                size_t wrappedRow = wrap(static_cast<int>(row) + dr, state.rows());
                size_t wrappedCol = wrap(static_cast<int>(col) + dc, state.cols());
                if(state(wrappedRow, wrappedCol) > 0.5) count++;
            }
        }

        return count;
    }
};
