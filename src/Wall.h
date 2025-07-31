#pragma once
#include <SFML/Graphics.hpp>

class Wall {
private:
    sf::RectangleShape shape;

public:
    Wall(const sf::Vector2f& pos, const sf::Vector2f& size);
    void draw(sf::RenderWindow& window) const;
    sf::FloatRect getBounds() const;
};
