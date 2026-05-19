#include <optional>

#include "render/sfml_renderer.hpp"

SFMLRenderer::SFMLRenderer(
    size_t rows,
    size_t cols,
    unsigned int cellSize
) : _rows{rows}, _cols{cols},
    _cellSize{cellSize},
    _window{
        sf::VideoMode{
            sf::Vector2u{
                static_cast<unsigned int>(cols * cellSize),
                static_cast<unsigned int>(rows * cellSize)
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
{}

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

    for(size_t r = 0; r < state.rows(); ++r) {
        for(size_t c = 0; c < state.cols(); ++c) {
            const double val = state(r, c);

            if(val <= 0.0) continue;

            const double fillVal = val * 255;
            _cell.setFillColor(sf::Color(fillVal, fillVal, fillVal));

            _cell.setPosition(
                sf::Vector2f{
                    static_cast<float>(c * _cellSize),
                    static_cast<float>(r * _cellSize)
                }
            );

            _window.draw(_cell);
        }
    }

    _window.display();
    sf::sleep(sf::milliseconds(100));
}
