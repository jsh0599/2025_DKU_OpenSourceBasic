#ifndef STATEFACTORY_HPP
#define STATEFACTORY_HPP

#include "state.hpp"

class InputManager;

class StateFactory {
public:
    template <typename T>
    static State* create(InputManager* manager) {
        State* state = new T(manager);
        state->initialize();
        return state;
    }
};

#endif // STATEFACTORY_HPP
