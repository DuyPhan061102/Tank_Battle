#ifndef ENEMY_H
#define ENEMY_H

#include "Tank.h"
#include <SFML/System.hpp>

class Enemy : public Tank
{
public:
    Enemy(float x, float y);
    void move(float dx, float dy) override;
    void update(float deltaTime) override;
    void draw(sf::RenderWindow &window) const override;

    bool isHit(const sf::FloatRect &bounds);

private:
    sf::Vector2f direction;
    float timeSinceDirectionChange;
};

#endif
