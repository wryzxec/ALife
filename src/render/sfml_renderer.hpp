#pragma once

#include <cstddef>

#include "SFML/Graphics.hpp"

#include "render/renderer.hpp"

class SFMLRenderer : public Renderer {
public:
    SFMLRenderer(size_t windowRows, size_t windowCols, unsigned int cellSize = 10);
    void render(const ALife& sim) override;
    void handleEvents() override;
    bool isOpen() const override;

private:
    static sf::Color colorFromState(const State& state, size_t r, size_t c);

    size_t _windowRows;
    size_t _windowCols;
    unsigned int _cellSize;

    sf::RenderWindow _window;
    sf::RectangleShape _cell;
    sf::RectangleShape _border;
};
