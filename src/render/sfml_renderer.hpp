#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

#include "SFML/Graphics.hpp"

#include "render/renderer.hpp"

class SFMLRenderer : public Renderer {
public:
    SFMLRenderer(size_t windowRows, size_t windowCols, unsigned int cellSize = 10);
    void render(const ALife& sim) override;
    void handleEvents() override;
    bool isOpen() const override;

private:
    sf::Color colorFromState(const State& state, size_t r, size_t c) const;

    void setPixel(size_t r, size_t c, sf::Color colour);
    void clearPixels();

    size_t _windowRows;
    size_t _windowCols;
    unsigned int _cellSize;

    sf::RenderWindow _window;

    std::vector<uint8_t> _pixels;
    sf::Texture _texture;
    sf::Sprite _sprite;

    sf::RectangleShape _border;
};
