#include "EnemyBoss.h"
#include <SFML/Graphics.hpp>

EnemyBoss::EnemyBoss(float x, float y) : Enemy(x, y, 50)
{
    body.setSize(sf::Vector2f(80.f, 80.f));
    body.setFillColor(sf::Color::Red);
    body.setPosition(x, y);
}


void EnemyBoss::update(float deltaTime)
{

    Enemy::update(deltaTime);

}

bool EnemyBoss::isHit(const sf::FloatRect& bounds)
{
    if (getBounds().intersects(bounds)) {
        takeDamage(1);
        return hp <= 0;
    }
    return false;
}