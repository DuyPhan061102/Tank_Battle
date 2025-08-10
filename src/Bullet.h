// bullet.h
#ifndef BULLET_H
#define BULLET_H

#include <SFML/Graphics.hpp>

class Bullet
{
private:
	sf::RectangleShape shape;
	sf::Vector2f direction;
	float speed;
	// skin đạn
	sf::Texture bulletTexture;
	sf::Sprite bulletSprite;

public:
	Bullet(const sf::Vector2f &startPos, const sf::Vector2f &dir, float speed = 100.f);
	void update(float dt);
	void draw(sf::RenderWindow &window) const;
	bool isOffScreen(const sf::RenderWindow &window) const;
	sf::FloatRect getBounds() const;
	const sf::Vector2f &getPosition() const;

};

#endif