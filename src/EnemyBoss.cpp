#include "EnemyBoss.h"
#include <SFML/Graphics.hpp>
#include <iostream>
EnemyBoss::EnemyBoss(float x, float y) : Enemy(x, y, 30)
{
    body.setSize(sf::Vector2f(60.f, 60.f));
    body.setFillColor(sf::Color::Red);

    // Thay ảnh riêng cho EnemyBoss
    if (!tankTexture.loadFromFile("assets/Images/tank5.png"))
        std::cout << "❌ Không thể tải boss.png\n";
    else
    {
        tankSprite.setTexture(tankTexture);
        tankSprite.setOrigin(tankTexture.getSize().x / 2.f, tankTexture.getSize().y / 2.f);
        tankSprite.setScale(0.7f, 0.7f); // boss to hơn
    }
    body.setPosition(x, y);
    shootCooldown = 1.0f; // Bắn nhanh hơn
    shootRange = 400.0f;  // Tầm bắn xa hơn
}

void EnemyBoss::update(float deltaTime)
{
    std::vector<Wall> emptyWalls;
    sf::Vector2f dummyPlayerPos(0.0f, 0.0f);
    update(deltaTime, emptyWalls, dummyPlayerPos);
}

void EnemyBoss::update(float deltaTime, const std::vector<Wall>& walls, const sf::Vector2f& playerPos)
{
    speed = 25.f;
    Enemy::update(deltaTime, walls, playerPos);
}

bool EnemyBoss::isHit(const sf::FloatRect &bounds)
{
    if (getBounds().intersects(bounds))
    {
        takeDamage(1);
        return true; // Sửa lại trả về true nếu bị trúng
    }
    return false;
}