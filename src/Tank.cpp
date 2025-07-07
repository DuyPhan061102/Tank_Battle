// Tank.cpp
#include "Tank.h"

Tank::Tank() {
    body.setSize(sf::Vector2f(40.f, 40.f));
    body.setFillColor(sf::Color::White);
    body.setPosition(100.f, 100.f);
    speed = 100.f;
}

void Tank::move(float dx, float dy) {
    body.move(dx, dy);
}
void Tank::draw(sf::RenderWindow& window) {
    window.draw(body);
}

sf::Vector2f Tank::getPosition() const {
    return body.getPosition();
}