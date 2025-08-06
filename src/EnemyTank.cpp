#include "EnemyTank.h"
#include <cmath>   
#include <iostream>
EnemyTank::EnemyTank(float x, float y) : Enemy(x, y, 8) {
    // Thay ảnh riêng cho EnemyTank
    if (!tankTexture.loadFromFile("assets/Images/tank3.png"))
        std::cout << "❌ Không thể tải tank2.png\n";
    else {
        tankSprite.setTexture(tankTexture);
        tankSprite.setOrigin(tankTexture.getSize().x / 2.f, tankTexture.getSize().y / 2.f);
        tankSprite.setScale(0.5f, 0.5f);
    }
    body.setFillColor(sf::Color(100, 100, 255));
    body.setSize(sf::Vector2f(30.f, 30.f)); // hoặc 40.f, 40.f nếu muốn lớn hơn
}


void EnemyTank::update(float deltaTime)
{
    std::vector<Wall> emptyWalls;
    update(deltaTime, emptyWalls);
}

void EnemyTank::update(float deltaTime, const std::vector<Wall>& walls)
{
    speed = 40.f;
    Enemy::update(deltaTime, walls);
}
