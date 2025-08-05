//Tank.cpp
#include "Tank.h"

Tank::Tank()
{
    body.setSize(sf::Vector2f(40.f, 40.f));
    body.setFillColor(sf::Color::White);
    body.setPosition(100.f, 100.f);
    speed = 100.f;
}

// ❌ Không cần hàm move vì nó là pure virtual và do lớp con định nghĩa

void Tank::draw(sf::RenderWindow &window) const {
    window.draw(body);
}

sf::Vector2f Tank::getPosition() const {
    return body.getPosition();
}

void Tank::setPosition(const sf::Vector2f &pos)
{
    body.setPosition(pos);
}

sf::FloatRect Tank::getBounds() const {
    return body.getGlobalBounds();
}
float Tank::getSpeed() const {
    return speed;
}