#include "Wall.h"

Wall::Wall(const sf::Vector2f& pos, const sf::Vector2f& size) {
    shape.setPosition(pos);
    shape.setSize(size);
    shape.setFillColor(sf::Color(100, 100, 100));
}

void Wall::draw(sf::RenderWindow& window) const {
    window.draw(shape);
}

sf::FloatRect Wall::getBounds() const {
    return shape.getGlobalBounds();
}
