#pragma once

#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <vector>

#include <nlohmann/json.hpp>

#include "patterns/pattern_preset.hpp"
#include "patterns/pattern.hpp"
#include "systems/system_type.hpp"

inline PatternPreset loadPatternPreset(const std::filesystem::path& path) {
    std::ifstream file(path);

    if(!file) throw std::runtime_error("Could not open pattern file: " + path.string());

    nlohmann::json data;
    file >> data;

    const std::string name = data.at("name").get<std::string>();
    const std::string system = data.at("system").get<std::string>();
    const std::string desc = data.value("desc", "");

    const size_t rows = data.at("rows").get<size_t>();
    const size_t cols = data.at("cols").get<size_t>();
    const size_t channels = data.at("channels").get<size_t>();
    const std::vector<double> values = data.at("values").get<std::vector<double>>();
    
    Pattern pattern(rows, cols, channels, values);

    return PatternPreset(name, systemTypeFromString(system), desc, std::move(pattern));
}
