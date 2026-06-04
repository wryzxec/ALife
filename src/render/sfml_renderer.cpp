#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <optional>

#include "render/sfml_renderer.hpp"

SFMLRenderer::SFMLRenderer(
    size_t windowRows,
    size_t windowCols,
    unsigned int cellSize
) : _windowRows{windowRows}, _windowCols{windowCols},
    _cellSize{cellSize},
    _window{
        sf::VideoMode{
            sf::Vector2u{
                static_cast<unsigned int>(windowCols * cellSize),
                static_cast<unsigned int>(windowRows * cellSize)
            }
        },
        "ALife"
    },
    _pixels(windowRows * windowCols * 4, 0), //4 channels rgba.
    _texture{
        sf::Vector2u{
            static_cast<unsigned int>(windowCols),
            static_cast<unsigned int>(windowRows)
        }
    },
    _sprite{_texture}
{
    _sprite.setScale(
        sf::Vector2f{
            static_cast<float>(cellSize),
            static_cast<float>(cellSize)
        }
    );

    _border.setFillColor(sf::Color::Transparent);
    _border.setOutlineColor(sf::Color{255, 255, 255, 50});
    _border.setOutlineThickness(1.0f);
}

void SFMLRenderer::handleEvents() {
    while(const std::optional event = _window.pollEvent()) {
        if(event->is<sf::Event::Closed>()) {
            _window.close();
        }
    }
}

bool SFMLRenderer::isOpen() const {
    return _window.isOpen();
}

void SFMLRenderer::render(const ALife& sim) {
    const State& state = sim.state();

    clearPixels();

    // centre the simulation in the window
    size_t rowOffset = (_windowRows / 2) - state.rows() / 2;
    size_t colOffset = (_windowCols / 2) - state.cols() / 2;

    // draw cell states
    for(size_t r = 0; r < state.rows(); ++r) {
        for(size_t c = 0; c < state.cols(); ++c) {
            setPixel(rowOffset + r, colOffset + c, colorFromState(state, r, c));
        }
    }

    _texture.update(_pixels.data());
    _window.clear(sf::Color::Black);
    _window.draw(_sprite);

    // position border around simulation
    _border.setPosition(
        sf::Vector2f{
            static_cast<float>(colOffset * _cellSize),
            static_cast<float>(rowOffset * _cellSize)
        }
    );
    _border.setSize(
        sf::Vector2f{
            static_cast<float>(state.cols() * _cellSize),
            static_cast<float>(state.rows() * _cellSize)
        }
    );

    _window.draw(_border);
    _window.display();
}

sf::Color SFMLRenderer::colorFromState(const State& state, size_t r, size_t c) const {
    const auto toByte = [](double value) {
        value = std::clamp(value, 0.0, 1.0);
        value = pow(value, 0.5); // make pixel values brighter (lower power -> greater brightness)
        return static_cast<uint8_t>(value * 255.0);
    };

    // render single channel systems with greyscale.
    if(state.channels() == 1) {
        const auto v = toByte(state(r, c, 0));
        return sf::Color(v, v, v);
    }
    
    // render multi-channel systems with rgb.
    // only supports rendering up to 3 channels, others are ignored.
    const double ch0 = state.channels() > 0 ? state(r, c, 0) : 0.0;
    const double ch1 = state.channels() > 1 ? state(r, c, 1) : 0.0;
    const double ch2 = state.channels() > 2 ? state(r, c, 2) : 0.0;
    
    return sf::Color(
        toByte(ch0),
        toByte(ch1),
        toByte(ch2)
    );
}
void SFMLRenderer::clearPixels() {
    for(size_t i = 0; i < _pixels.size(); i += 4) {
        _pixels[i] = 0;
        _pixels[i + 1] = 0;
        _pixels[i + 2] = 0;
        _pixels[i + 3] = 255;
    }
}

void SFMLRenderer::setPixel(size_t r, size_t c, sf::Color colour) {
    const size_t i = 4 * (r * _windowCols + c);
    _pixels[i] = colour.r;
    _pixels[i + 1] = colour.g;
    _pixels[i + 2] = colour.b;
    _pixels[i + 3] = colour.a;
}
