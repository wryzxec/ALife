cmake_minimum_required(VERSION 3.25)
project(alife LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

# sfml
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW TRUE
)
set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
set(SFML_STATIC_LIBRARIES OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(SFML)

# kissfft
FetchContent_Declare(
    kissfft
    GIT_REPOSITORY https://github.com/mborgerding/kissfft
    GIT_TAG master
    GIT_SHALLOW true
)
set(KISSFFT_TEST OFF CACHE BOOL "" FORCE)
set(KISSFFT_TOOLS OFF CACHE BOOL "" FORCE)
set(KISSFFT_PKGCONFIG OFF CACHE BOOL "" FORCE)
set(KISSFFT_STATIC ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(kissfft)

# nlohmann
FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
    GIT_SHALLOW true
)

FetchContent_MakeAvailable(nlohmann_json)

set(CMAKE_CXX_FLAGS "-O3 -Wall -Wextra")

add_executable(alife
    src/main.cpp
    src/app.cpp
    src/render/sfml_renderer.cpp
    src/config/sim_preset_loader.cpp
)

target_link_libraries(alife PRIVATE
    SFML::Graphics
    kissfft
    nlohmann_json::nlohmann_json
)

target_include_directories(alife PRIVATE
    ${PROJECT_SOURCE_DIR}/src
)

target_compile_definitions(alife PRIVATE)
