#pragma once

#include <string>
#include <utility>

#include "systems/system_type.hpp"
#include "patterns/pattern.hpp"

struct PatternPreset {
    std::string name;

    SystemType system;
    std::string desc;
    Pattern pattern;

    PatternPreset(
            std::string name_, SystemType system_, 
            std::string desc_, Pattern pattern_
    ) : name{std::move(name_)}, system{system_},
        desc{std::move(desc_)}, pattern{std::move(pattern_)}
    {}
};
