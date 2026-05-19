#pragma once

#include <cstddef>

#include "SFML/Graphics.hpp"

#include "render/renderer.hpp"

class SFMLRenderer : public Renderer {
public:
    SFMLRenderer(size_t rows, size_t cols, unsigned int cellSize = 10);
    void render(const ALife& sim) override;
    void handleEvents() override;
    bool isOpen() const override;
private:
    size_t _rows;
    size_t _cols;
    unsigned int _cellSize;

    sf::RenderWindow _window;
    sf::RectangleShape _cell;
};
