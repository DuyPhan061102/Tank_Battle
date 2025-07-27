// Bullet.cpp
#include "Bullet.h"

Bullet::Bullet(const sf::Vector2f &startPos, const sf::Vector2f &dir, float spd)
	: direction(dir), speed(spd)
{
	shape.setSize(sf::Vector2f(6.f, 6.f));
	shape.setFillColor(sf::Color::Yellow);
	shape.setPosition(startPos);
}

void Bullet::update(float dt)
{
	shape.move(direction * speed * dt);
}

void Bullet::draw(sf::RenderWindow &window) const
{
	window.draw(shape);
}

bool Bullet::isOffScreen(const sf::RenderWindow &window) const
{
	sf::Vector2u winSize = window.getSize();
	sf::Vector2f pos = shape.getPosition();
	return (pos.x < 0 || pos.x > winSize.x || pos.y < 0 || pos.y > winSize.y);
}

sf::FloatRect Bullet::getBounds() const
{
	return shape.getGlobalBounds();
}

const sf::Vector2f &Bullet::getPosition() const
{
	return shape.getPosition();
}