#include "Wall.h"

Wall::Wall(const sf::Vector2f& pos, const sf::Vector2f& size, int durability)
    : maxHit(durability), hitCount(0)
{
    shape.setPosition(pos);
    shape.setSize(size);

    if (texture.loadFromFile("assets/Images/wall.png")) {
        shape.setTexture(&texture); // 👈 gán texture nếu load thành công
    } else {
        shape.setFillColor(sf::Color(150, 150, 150)); // fallback
    }
}


void Wall::draw(sf::RenderWindow& window) const {
    window.draw(shape);
}

sf::FloatRect Wall::getBounds() const {
    return shape.getGlobalBounds();
}

void Wall::takeDamage() {
    hitCount++;
}

bool Wall::isDestroyed() const {
    return hitCount >= maxHit;
}

void Wall::setTexture(const sf::Texture* tex) {
    shape.setTexture(tex);
}





