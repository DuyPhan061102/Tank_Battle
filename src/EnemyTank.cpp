#include "EnemyTank.h"
#include <cmath>

EnemyTank::EnemyTank(float x, float y) : Enemy(x, y, 3) {
    body.setFillColor(sf::Color(100, 100, 255));
}


void EnemyTank::update(float deltaTime) {
    speed = 40.f;
    Enemy::update(deltaTime);
}
