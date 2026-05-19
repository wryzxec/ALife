#include "alife/updaterule.hpp"

class ConwayRule : public UpdateRule {
public:
    Field apply(const State& state, const Field& nbrCount) {
        Field next(state.rows(), state.cols(), state.channels());

        for(size_t r = 0; r < state.rows(); ++r) {
            for(size_t c = 0; c < state.cols(); ++c) {
                const bool alive = state(r, c) > 0.5;
                const int count = static_cast<int>(nbrCount(r, c));
                
                if(alive && (count == 2 || count == 3)) {
                    next(r, c) = 1.0;
                } else if(!alive && count == 3) {
                    next(r, c) = 1.0;
                } else {
                    next(r, c) = 0.0;
                }


            }
        }

        return next;
    }
};
