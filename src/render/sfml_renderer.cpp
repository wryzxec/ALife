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
    _cell{
        sf::Vector2f{
            static_cast<float>(cellSize),
            static_cast<float>(cellSize)
        }
    }
{
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
    
    _window.clear(sf::Color::Black);

    // centre the simulation in the window
    size_t rowOffset = (_windowRows / 2) - state.rows() / 2;
    size_t colOffset = (_windowCols / 2) - state.cols() / 2;

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

    // draw cell states
    for(size_t r = 0; r < state.rows(); ++r) {
        for(size_t c = 0; c < state.cols(); ++c) {
            const double val = state(r, c);

            if(val <= 0.0) continue;

            const double fillVal = val * 255;
            _cell.setFillColor(sf::Color(fillVal, fillVal, fillVal));

            _cell.setPosition(
                sf::Vector2f{
                    static_cast<float>(_cellSize * (colOffset + c)),
                    static_cast<float>(_cellSize * (rowOffset + r))
                }
            );

            _window.draw(_cell);
        }
    }

    _window.draw(_border);

    _window.display();
    sf::sleep(sf::milliseconds(100));
}
