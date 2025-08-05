// Bullet.cpp
#include "Bullet.h"
#include <cmath>

Bullet::Bullet(const sf::Vector2f &startPos, const sf::Vector2f &dir, float spd)
	: direction(dir), speed(spd)
{
	shape.setSize(sf::Vector2f(12.f, 12.f)); // hoặc 10.f, 10.f hoặc lớn hơn tuỳ ý
	shape.setFillColor(sf::Color::Yellow);
	shape.setPosition(startPos);
	// skin đạn
	if (bulletTexture.loadFromFile("assets/Images/bullet1.png"))
	{
		bulletSprite.setTexture(bulletTexture);
		bulletSprite.setOrigin(bulletTexture.getSize().x / 2.f, bulletTexture.getSize().y / 2.f);
		bulletSprite.setScale(0.6f, 0.6f); // hoặc 0.7f, 0.8f tuỳ ý
	}
}

void Bullet::update(float dt)
{
	sf::Vector2f movement = direction * speed * dt;
	shape.move(movement); // đơn giản chỉ di chuyển đạn

	// Cập nhật vị trí sprite theo shape
	bulletSprite.setPosition(
		shape.getPosition().x + shape.getSize().x / 2,
		shape.getPosition().y + shape.getSize().y / 2);

	// Nếu muốn quay sprite theo hướng bay của đạn
	float angle = std::atan2(direction.y, direction.x) * 180.f / 3.14159265f;
	bulletSprite.setRotation(angle);
}

void Bullet::draw(sf::RenderWindow &window) const
{
	if (bulletTexture.getSize().x > 0)
		window.draw(bulletSprite);
	else
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