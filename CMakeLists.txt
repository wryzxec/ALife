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

set(CMAKE_CXX_FLAGS "-O3 -Wall -Wextra")

add_executable(alife
    src/main.cpp
    src/app.cpp
    src/render/sfml_renderer.cpp
)

target_link_libraries(alife PRIVATE
    SFML::Graphics
)

target_include_directories(alife PRIVATE
    ${PROJECT_SOURCE_DIR}/src
)

target_compile_definitions(alife PRIVATE)
