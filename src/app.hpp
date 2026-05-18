#pragma once

#include <memory>

#include "alife/alife.hpp"

class App {
public:
    App(std::unique_ptr<ALife> sim);

    void run();
    void stop();
private:
    bool _running;
    std::unique_ptr<ALife> _sim;
};
