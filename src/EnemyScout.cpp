#include "EnemyScout.h"
EnemyScout::EnemyScout(float x, float y) : Enemy(x, y, 3) {
    body.setFillColor(sf::Color::Yellow);
}


void EnemyScout::update(float deltaTime) {
   
    speed = 90.f;
    Enemy::update(deltaTime);
}

