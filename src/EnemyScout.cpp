#include "EnemyScout.h"
#include <iostream>
EnemyScout::EnemyScout(float x, float y) : Enemy(x, y, 3)
{
    // Thay ảnh riêng cho EnemyScout
    if (!tankTexture.loadFromFile("assets/Images/tank4.png"))
        std::cout << "❌ Không thể tải scout.png\n";
    else
    {
        tankSprite.setTexture(tankTexture);
        tankSprite.setOrigin(tankTexture.getSize().x / 2.f, tankTexture.getSize().y / 2.f);
        tankSprite.setScale(0.5f, 0.5f);
    }
    body.setFillColor(sf::Color::Yellow);
}

void EnemyScout::update(float deltaTime)
{
    std::vector<Wall> emptyWalls;
    update(deltaTime, emptyWalls);
}

void EnemyScout::update(float deltaTime, const std::vector<Wall>& walls)
{
    speed = 90.f;
    Enemy::update(deltaTime, walls);
}
