#pragma once

#include <filesystem>

#include "config/sim_preset.hpp"

SimulationPreset loadSimulationPreset(const std::filesystem::path& path);
