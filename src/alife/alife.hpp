#pragma once

#include <memory>

#include "core/state.hpp"
#include "alife/observation.hpp"
#include "alife/updaterule.hpp"
#include "alife/integrator.hpp"
#include "alife/constraint.hpp"

class ALife {
public:
    ALife(State initialState,
          std::unique_ptr<Observation> observation,
          std::unique_ptr<UpdateRule> updateRule,
          std::unique_ptr<Integrator> integrator,
          std::unique_ptr<Constraint> constraint
    ) : _state{std::move(initialState)},
        _observation{std::move(observation)},
        _updateRule{std::move(updateRule)},
        _integrator{std::move(integrator)},
        _constraint{std::move(constraint)}
    {}

    virtual ~ALife() = default;
    const State& state() const { return _state; }
    virtual void step() {
        Field observed = _observation->observe(_state);
        Field update = _updateRule->apply(_state, observed);

        _integrator->integrate(_state, update);
        _constraint->apply(_state);
    }
private:
    State _state;

    std::unique_ptr<Observation> _observation;
    std::unique_ptr<UpdateRule> _updateRule;
    std::unique_ptr<Integrator> _integrator;
    std::unique_ptr<Constraint> _constraint;
};
