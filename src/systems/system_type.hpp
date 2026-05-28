#pragma once

#include <array>
#include <string>
#include <string_view>

enum class SystemType {
    Conway,
    LargerThanLife,
    SmoothLife,
    Lenia
};

struct SystemTypeEntry {
    std::string_view name;
    SystemType type;
};

inline constexpr std::array<SystemTypeEntry, 4> systemTypeEntries{{
    {"conway", SystemType::Conway},
    {"larger_than_life", SystemType::LargerThanLife},
    {"smoothlife", SystemType::SmoothLife},
    {"lenia", SystemType::Lenia}
}};

inline std::string validSystemTypesString() {
    std::string str;
    for(std::size_t i = 0; i < systemTypeEntries.size(); ++i) {
        str += systemTypeEntries[i].name;
        if(i + 1 < systemTypeEntries.size()) {
            str += ' ';
        }
    }
    return str;
}

inline SystemType systemTypeFromString(std::string_view str) {
    for(const auto& entry : systemTypeEntries) {
        if(entry.name == str) {
            return entry.type;
        }
    }

    throw std::invalid_argument("Unknown system type: " + std::string(str) +
            ". Valid types are: " + validSystemTypesString());
}
